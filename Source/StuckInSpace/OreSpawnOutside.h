// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "OreSpawnOutside.generated.h"

class AOreNode;

UCLASS()
class STUCKINSPACE_API AOreSpawnOutside : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOreSpawnOutside();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere, Category="Ore")
	TSubclassOf<AOreNode> Ore;


	FTimerHandle RespawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnOre();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnCall();
};
