// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachineBase.h"
#include "NiagaraComponent.h"
#include "OxygenGenerator.generated.h"

class AMyCharacter;

/**
 * 
 */
UCLASS()
class STUCKINSPACE_API AOxygenGenerator : public AMachineBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AOxygenGenerator();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UNiagaraComponent* SteamEffect;

	UFUNCTION(BlueprintCallable)
	bool RefillOxygen(AMyCharacter* Character);
};
