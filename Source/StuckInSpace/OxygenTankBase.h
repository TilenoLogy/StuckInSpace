// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OxygenTankBase.generated.h"

UCLASS()
class STUCKINSPACE_API AOxygenTankBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOxygenTankBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	int32 OxygenCapacity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	float DrainRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	FName OxygenTankId;

};
