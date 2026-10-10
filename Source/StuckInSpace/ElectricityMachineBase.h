// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnappableBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ElectricityMachineBase.generated.h"

UCLASS()
class STUCKINSPACE_API AElectricityMachineBase : public ASnappableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectricityMachineBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Damage_Implementation() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void DestroyConnectedWires();

	UFUNCTION(BlueprintCallable, Category = "Electricity")
	bool HasWireTo(const AElectricityMachineBase* OtherMachine) const;

	bool bDestroyingConnectedWires = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ANetworkManager* PowerNetwork;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battery")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PowerProduction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PowerConsumption = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BatteryCapacity = 0.f;





};
