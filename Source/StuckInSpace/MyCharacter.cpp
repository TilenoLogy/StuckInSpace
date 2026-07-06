// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "GameFramework/PlayerController.h"


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
			JumpAction,
			ETriggerEvent::Started,
			this,
			&AMyCharacter::StartJump);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&AMyCharacter::StopJump);

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


void AMyCharacter::UseItem(FString ItemName) {
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemName == ItemName)
		{
			Inventory[i].Amount--;

			if (Inventory[i].Amount <= 0)
			{
				Inventory.RemoveAt(i);
			}

			break;
		}
	}
}

bool AMyCharacter::FindItem(int32 Amount, FString ItemName) {
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemName == ItemName)
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


bool AMyCharacter::RefillOxygen() {
	if (FindItem(1, "Coal Chunk") && FindItem(1, "Ice Cubes")) {
		if (Oxygen <= 0.9)
			Oxygen += 0.1f;
		else {
			Oxygen = 1.0f;
		}
		UseItem("Coal Chunk");
		UseItem("Ice Cubes");
		return true;
	}
	return false;
}