// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

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
	CheckLookedAtObject();

	if (bIsRunning)
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
	if (!IsValid(LookedAtActor) || !LookedAtActor->Implements<UBreakInterface>())
	{
		return;
	}

	if (APlaceableBase* Placeable = Cast<APlaceableBase>(LookedAtActor))
	{
		if (!Placeable->DestroyedItemID.IsNone())
		{
			AddItem(Placeable->DestroyedItemID, 1);
		}
	}

	IBreakInterface::Execute_Damage(LookedAtActor);
	LookedAtActor = nullptr;
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
		UE_LOG(LogTemp, Warning, TEXT("ReaLaUCH"));
		Oxygen -= 0.1f;
	}

	if (Oxygen <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("You Died"));
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

void AMyCharacter::AddItem(FName ItemID, int32 Amount) {
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

void AMyCharacter::UpgradeStrength()
{
	Strength++;
}


void AMyCharacter::AddOxygen()
{
	Oxygen = FMath::Min(Oxygen + 0.5f, 1.0f);
}

//bool AMyCharacter::RefillOxygenAtLookedAtGenerator()
//{
//	AOxygenGenerator* Generator = Cast<AOxygenGenerator>(LookedAtActor);
//	return Generator && Generator->RefillOxygen(this);
//}