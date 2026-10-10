// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkManager.generated.h"

class AElectricityMachineBase;

UCLASS()
class STUCKINSPACE_API ANetworkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite)
	TArray<AElectricityMachineBase*> Devices;

	UPROPERTY(BlueprintReadWrite)
	float TotalProduction = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float TotalConsumption = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float TotalCapacity = 0.f;

	UFUNCTION(BlueprintCallable)
	void RecalculateNetwork();

	UFUNCTION(BlueprintCallable)
	static void RebuildAllNetworks(UWorld* World);

	static void RebuildAllNetworksNextTick(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void RegisterDevice(AElectricityMachineBase* Device);

	void RefreshDevices();

	void TransferEnergy(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void MergeNetwork(ANetworkManager* NetworkA,
		ANetworkManager* NetworkB);
};
