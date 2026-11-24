// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableBase.h"
#include "TimerManager.h"
#include "AutoMinersBase.generated.h"

class AMyCharacter;
class AOreSpawnOutside;
class UStaticMeshComponent;

UCLASS()
class STUCKINSPACE_API AAutoMinersBase : public APlaceableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoMinersBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Miner")
	UStaticMeshComponent* MinerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	AOreSpawnOutside* SpawnPoint = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bNeedsGroundSnap = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Damage_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Miner")
	FName OreItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Miner")
	float MiningInterval = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Miner")
	int32 StoredAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Miner")
	int32 StorageCapacity = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Miner")
	bool IsMining = false;

	UFUNCTION(BlueprintCallable, Category = "Miner")
	void StartMining();

	UFUNCTION(BlueprintCallable, Category = "Miner")
	void StopMining();

	UFUNCTION(BlueprintCallable, Category = "Miner")
	void MineOre();

	UFUNCTION(BlueprintCallable, Category = "Miner")
	void CollectOre(AMyCharacter* Character);

	FTimerHandle MinerTimer;

};
