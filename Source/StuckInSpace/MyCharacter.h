// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class AActor;

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	Normal,
	Inventory,
	Building,
	Destruction
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 0;
};

USTRUCT(BlueprintType)
struct FMiningToolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Tier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage = 1;
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

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CheckLookedAtObject();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetMachineHighlight(AActor* Actor, bool bHighlighted);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void DamageLookedAtMachine();

	UFUNCTION(BlueprintCallable, Category = "Placement")
	static bool SnapActorToGround(AActor* Actor, float TraceDistance = 1000.f);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DamageAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);



	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;
	
	UFUNCTION()
	void StartRunning(const FInputActionValue& Value);

	UFUNCTION()
	void StopRunning(const FInputActionValue& Value);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Strength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining")
	FName EquippedToolItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining")
	TArray<FMiningToolData> MiningTools;

	bool bIsRunning = false;
	float StaminaDebugTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionDistance = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* LookedAtActor = nullptr;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Oxygen=1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bSaveZone = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItem> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Modes")
	EPlayerMode PlayerMode = EPlayerMode::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 MaxWeight = 100;


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(FName ItemID, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemID, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Mining")
	bool EquipMiningTool(FName ItemID);


	UFUNCTION(BlueprintCallable, Category="Oxygen")
	void UseOxygen();

	UFUNCTION(BlueprintCallable, Category="Oxygen")
	void AddOxygen();

	//UFUNCTION(BlueprintCallable, Category="Oxygen")
	//bool RefillOxygenAtLookedAtGenerator();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool FindItem(int32 Amount, FName ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 GetItemAmount(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpgradeStrength();

	UFUNCTION(BlueprintCallable, Category = "Mining")
	int32 GetMiningToolTier() const;

	UFUNCTION(BlueprintCallable, Category = "Mining")
	int32 GetMiningDamage() const;

	// Returns the total weight of all items in the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetTotalWeight() const;
	// Returns the weight of a specific item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetItemWeight(FName ItemID) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UDataTable* ItemDataTable;
};
