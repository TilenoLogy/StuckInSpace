// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"


class UInputMappingContext;
class UInputAction;


USTRUCT(BlueprintType)
struct FItem{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 0;
};

UCLASS()
class STUCKINSPACE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);



	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Oxygen=1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bSaveZone = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItem> Inventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(FString ItemName);


	UFUNCTION(BlueprintCallable, Category="Oxygen")
	void UseOxygen();

	UFUNCTION(BlueprintCallable, Category="Oxygen")
	bool RefillOxygen();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool FindItem(int32 Amount, FString ItemName);

};
