// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "ItemData.h"
#include "Engine/DataTable.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "BreakInterface.h"
#include "PlaceableBase.h"
#include "OxygenGenerator.h"
#include "OxygenTankData.h"
#include "DamageOverlayWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (DamageOverlayWidgetClass)
	{
		DamageOverlayWidget = CreateWidget<UDamageOverlayWidget>(GetWorld(), DamageOverlayWidgetClass);
		if (DamageOverlayWidget)
		{
			DamageOverlayWidget->AddToViewport();
			UpdateDamageOverlay();
		}
	}

	UpdateDamageOverlay();
	
	if (APlayerController* PC =
		Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(
					DefaultMappingContext,
					0
				);
			}
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerMode == EPlayerMode::Destruction)
	{
		CheckLookedAtObject();
	}
	else if (LookedAtActor)
	{
		SetMachineHighlight(LookedAtActor, false);
		LookedAtActor = nullptr;
	}

	const bool bIsMoving = GetVelocity().SizeSquared2D() > FMath::Square(1.f);

	if (bIsRunning && bIsMoving)
	{
		Stamina -= 20.f * DeltaTime;

		if (Stamina <= 0.f)
		{
			Stamina = 0.f;
			StopRunning(FInputActionValue());
		}
	}
	else
	{
		Stamina += 10.f * DeltaTime;
		Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
	}
}

void AMyCharacter::CheckLookedAtObject()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * InteractionDistance;
	FHitResult Hit;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LookedAtObject), true, this);

	AActor* NewTarget = nullptr;
	if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		APlaceableBase* Placeable = Cast<APlaceableBase>(Hit.GetActor());
		if (Placeable && Placeable->bCanBeBroken)
		{
			NewTarget = Placeable;
		}
	}

	if (NewTarget == LookedAtActor)
	{
		return;
	}

	if (LookedAtActor)
	{
		SetMachineHighlight(LookedAtActor, false);
	}

	LookedAtActor = NewTarget;
	if (LookedAtActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("LookingAtMachine2"));
		SetMachineHighlight(LookedAtActor, true);
	}
}

void AMyCharacter::SetMachineHighlight(AActor* Actor, bool bHighlighted)
{
	if (!Actor)
	{
		return;
	}

	TArray<UMeshComponent*> Components;
	Actor->GetComponents<UMeshComponent>(Components);

	for (UMeshComponent* Component : Components)
	{
		Component->SetRenderCustomDepth(bHighlighted);
		if (bHighlighted)
		{
			Component->SetCustomDepthStencilValue(1);
		}
	}
}

void AMyCharacter::DamageLookedAtMachine()
{
	if (PlayerMode != EPlayerMode::Destruction)
	{
		return;
	}

	CheckLookedAtObject();

	if (!IsValid(LookedAtActor) || !LookedAtActor->Implements<UBreakInterface>())
	{
		return;
	}

	if (APlaceableBase* Placeable = Cast<APlaceableBase>(LookedAtActor))
	{
		if (!Placeable->bCanBeBroken)
		{
			return;
		}

		if (!Placeable->DestroyedItemID.IsNone())
		{
			AddItem(Placeable->DestroyedItemID, 1);
		}
	}

	AActor* TargetToDamage = LookedAtActor;
	SetMachineHighlight(TargetToDamage, false);
	LookedAtActor = nullptr;
	IBreakInterface::Execute_Damage(TargetToDamage);
}

bool AMyCharacter::SnapActorToGround(AActor* Actor, float TraceDistance)
{
	if (!IsValid(Actor) || !Actor->GetWorld() || TraceDistance <= 0.f)
	{
		return false;
	}

	UMeshComponent* MeshComponent = nullptr;
	TArray<UMeshComponent*> MeshComponents;
	Actor->GetComponents<UMeshComponent>(MeshComponents);

	for (UMeshComponent* Candidate : MeshComponents)
	{
		if (Candidate && Candidate->IsVisible())
		{
			MeshComponent = Candidate;
			break;
		}
	}

	if (!MeshComponent)
	{
		return false;
	}

	const FBoxSphereBounds Bounds = MeshComponent->Bounds;
	const float BottomOffset =
		(Bounds.Origin - Actor->GetActorLocation()).Z - Bounds.BoxExtent.Z;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SnapActorToGround), false, Actor);
	FHitResult GroundHit;
	const FVector TraceStart = Bounds.Origin + FVector(0.f, 0.f, Bounds.BoxExtent.Z + 100.f);
	const FVector TraceEnd = Bounds.Origin - FVector(0.f, 0.f, Bounds.BoxExtent.Z + TraceDistance);

	if (!Actor->GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams))
	{
		return false;
	}

	FVector CorrectedLocation = Actor->GetActorLocation();
	CorrectedLocation.Z = GroundHit.ImpactPoint.Z - BottomOffset + 1.f;
	Actor->SetActorLocation(CorrectedLocation);
	return true;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AMyCharacter::Move
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AMyCharacter::Look
		);

		EnhancedInputComponent->BindAction(
			DamageAction,
			ETriggerEvent::Started,
			this,
			&AMyCharacter::DamageLookedAtMachine);


		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&AMyCharacter::StartJump);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&AMyCharacter::StopJump);

		EnhancedInputComponent->BindAction(
			RunAction,
			ETriggerEvent::Triggered,
			this,
			&AMyCharacter::StartRunning);

		EnhancedInputComponent->BindAction(
			RunAction,
			ETriggerEvent::Started,
			this,
			&AMyCharacter::StartRunning);

		EnhancedInputComponent->BindAction(
			RunAction,
			ETriggerEvent::Completed,
			this,
			&AMyCharacter::StopRunning);

	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(
		GetActorForwardVector(),
		MovementVector.Y
	);

	AddMovementInput(
		GetActorRightVector(),
		MovementVector.X
	);
}


void AMyCharacter::Look(const FInputActionValue& Value)
{
	
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);

	AddControllerPitchInput(LookAxis.Y);
}


void AMyCharacter::StartJump(const FInputActionValue& Value)
{
	Jump();
}

void AMyCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void AMyCharacter::StartRunning(const FInputActionValue& Value)
{
	if (Stamina > 0.f)
	{
		bIsRunning = true;
		SetActorTickEnabled(true);
		
		UE_LOG(LogTemp, Warning, TEXT("StartRunning called"));
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		
	}
}


void AMyCharacter::StopRunning(const FInputActionValue& Value)
{
	bIsRunning = false;
	UE_LOG(LogTemp, Warning, TEXT("StopRunning called"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void AMyCharacter::UseOxygen() {
	UE_LOG(LogTemp, Warning, TEXT("aUCH"));
	if (!bSaveZone && Oxygen > 0.0f) {
		Oxygen = FMath::Max(0.0f, Oxygen - CurrentDrainRate);
		UE_LOG(LogTemp, Warning, TEXT("Oxygen remaining: %.2f"), Oxygen);
		UpdateDamageOverlay();

	}

	if (Oxygen <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("You Died"));

		const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
		if (!CurrentLevelName.IsEmpty())
		{
			UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
		}
	}
}


bool AMyCharacter::UseItem(FName ItemID, int32 Amount) {
	if (Amount <= 0)
	{
		return false;
	}

	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemID == ItemID)
		{
			if (Inventory[i].Amount < Amount)
			{
				return false;
			}

			Inventory[i].Amount -= Amount;

			if (Inventory[i].Amount <= 0)
			{
				Inventory.RemoveAt(i);
			}

			return true;
		}
	}

	return false;
}

float AMyCharacter::GetItemWeight(FName ItemID) const {
	if (!ItemDataTable) {
		UE_LOG(LogTemp, Warning, TEXT("DT doesn't exist"))
		return 1.0f;
	}
	FString Context;
	FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemID, Context);
	if (Row) { 
		UE_LOG(LogTemp, Warning, TEXT("Row found"))
		return Row->Weight; }
	return 1.0f;
}

float AMyCharacter::GetTotalWeight() const {
	float Total = 0.0f;
	for (const FItem& Item : Inventory) {
		Total += GetItemWeight(Item.ItemID) * Item.Amount;
	}
	return Total;
}

void AMyCharacter::AddItem(FName ItemID, int32 Amount) {
	if (ItemID.IsNone() || Amount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot add item: invalid ItemID or amount (ItemID=%s, Amount=%d)"),
			*ItemID.ToString(), Amount);
		return;
	}

	float ItemWeight = GetItemWeight(ItemID);
	float NewTotalWeight = GetTotalWeight() + ItemWeight * Amount;
	if (NewTotalWeight > MaxWeight) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot add item %s: would exceed max weight!"), *ItemID.ToString());
		return;
	}

	if (EquipMiningTool(ItemID))
	{
		UE_LOG(LogTemp, Log, TEXT("Equipped mining tool: %s"), *ItemID.ToString());
	}

	if (ItemDataTable)
	{
		FString Context;
		if (const FItemData* ItemRow = ItemDataTable->FindRow<FItemData>(ItemID, Context))
		{
			if (ItemRow->Category == FName(TEXT("Tank")))
			{
				UpgradeOxygenTank();
			}
		}
	}

	for (FItem& tmp : Inventory) {
		if (tmp.ItemID == ItemID) {
			tmp.Amount += Amount;
			return;
		}
	}

	FItem tmp;
	tmp.ItemID = ItemID;
	tmp.Amount = Amount;
	Inventory.Add(tmp);
}


bool AMyCharacter::EquipMiningTool(FName ItemID)
{
	if (ItemID.IsNone())
	{
		return false;
	}

	for (const FMiningToolData& Tool : MiningTools)
	{
		if (Tool.ItemID == ItemID)
		{
			EquippedToolItemID = ItemID;
			return true;
		}
	}

	return false;
}


bool AMyCharacter::FindItem(int32 Amount, FName ItemID) {
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemID == ItemID)
		{
			

			if (Inventory[i].Amount >= Amount)
			{
				return true;
			}

			break;
		}
	}
	return false;
}

int32 AMyCharacter::GetItemAmount(FName ItemID) const
{
	for (const FItem& Item : Inventory)
	{
		if (Item.ItemID == ItemID)
		{
			return Item.Amount;
		}
	}

	return 0;
}

void AMyCharacter::UpgradeStrength()
{
	Strength++;
}

int32 AMyCharacter::GetMiningToolTier() const
{
	for (const FMiningToolData& Tool : MiningTools)
	{
		if (Tool.ItemID == EquippedToolItemID)
		{
			return FMath::Max(0, Tool.Tier);
		}
	}

	return FMath::Max(0, Strength);
}

int32 AMyCharacter::GetMiningDamage() const
{
	for (const FMiningToolData& Tool : MiningTools)
	{
		if (Tool.ItemID == EquippedToolItemID)
		{
			return FMath::Max(0, Tool.Damage);
		}
	}

	return 1;
}


void AMyCharacter::AddOxygen()
{
	Oxygen = FMath::Min(Oxygen + 0.5f, MaxOxygen);
	UpdateDamageOverlay();
}

void AMyCharacter::UpdateDamageOverlay()
{
	const float OxygenRatio = MaxOxygen > 0.0f
		? FMath::Clamp(Oxygen / MaxOxygen, 0.0f, 1.0f)
		: 0.0f;
	const float MissingOxygenRatio = 1.0f - OxygenRatio;
	if (DamageOverlayWidget)
	{
		DamageOverlayWidget->SetDamageOpacity(MissingOxygenRatio * MaxDamageOverlayOpacity);
	}

	if (MissingOxygenRatio > 0.5f) {
		const float CurrentBlurStrength = (MissingOxygenRatio-0.5f) * BlurStrength;
		OnOxygenChanged(OxygenRatio, CurrentBlurStrength);
	}
}

bool AMyCharacter::UpgradeOxygenTank()
{
	if (!OxygenTanksDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Oxygen tanks data table is not set."));
		return false;
	}

	const FString CurrentTankString = CurrentTankID.ToString();
	const FString Prefix = TEXT("OxygenTank");
	if (!CurrentTankString.StartsWith(Prefix))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid current oxygen tank ID: %s"), *CurrentTankID.ToString());
		return false;
	}

	const int32 CurrentTankNumber = FCString::Atoi(*CurrentTankString.RightChop(Prefix.Len()));
	if (CurrentTankNumber <= 0)
	{
		return false;
	}

	const FName NextTankID(*FString::Printf(TEXT("%s%d"), *Prefix, CurrentTankNumber + 1));
	FString Context;
	const FOxygenTankData* NextTank = OxygenTanksDataTable->FindRow<FOxygenTankData>(NextTankID, Context);
	if (!NextTank)
	{
		UE_LOG(LogTemp, Log, TEXT("No oxygen tank upgrade found for %s."), *CurrentTankID.ToString());
		return false;
	}

	CurrentTankID = NextTank->TankID.IsNone() ? NextTankID : NextTank->TankID;
	MaxOxygen = NextTank->OxygenCapacity;
	CurrentDrainRate = NextTank->DrainRate;
	Oxygen = FMath::Min(Oxygen, MaxOxygen);

	UE_LOG(LogTemp, Log, TEXT("Upgraded oxygen tank to %s. Max oxygen: %.2f, drain rate: %.2f"),
		*CurrentTankID.ToString(), MaxOxygen, CurrentDrainRate);
	return true;
}

//bool AMyCharacter::RefillOxygenAtLookedAtGenerator()
//{
//	AOxygenGenerator* Generator = Cast<AOxygenGenerator>(LookedAtActor);
//	return Generator && Generator->RefillOxygen(this);
//}