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
#include "MachineBase.h"
#include "Components/MeshComponent.h"


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
		AMachineBase* Machine = Cast<AMachineBase>(Hit.GetActor());
		if (Machine && Machine->bCanBeBroken)
		{
			NewTarget = Machine;
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

	IBreakInterface::Execute_Damage(LookedAtActor);
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


void AMyCharacter::UseItem(FName ItemID, int32 Amount) {
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemID == ItemID && Inventory[i].Amount-Amount>=0)
		{
			Inventory[i].Amount = Inventory[i].Amount-Amount;

			if (Inventory[i].Amount <= 0)
			{
				Inventory.RemoveAt(i);
			}

			break;
		}
	}
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


bool AMyCharacter::RefillOxygen() {
	if (FindItem(1, "Coal Chunk") && FindItem(1, "Ice Cubes")) {
		if (Oxygen <= 0.9)
			Oxygen += 0.1f;
		else {
			Oxygen = 1.0f;
		}
		UseItem("Coal Chunk", 1);
		UseItem("Ice Cubes", 1);
		return true;
	}
	return false;
}