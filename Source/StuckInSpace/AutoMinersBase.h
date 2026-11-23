// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "AutoMinersBase.generated.h"

class AMyCharacter;
class UStaticMeshComponent;

UCLASS()
class STUCKINSPACE_API AAutoMinersBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoMinersBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Miner")
	UStaticMeshComponent* MinerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
