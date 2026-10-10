// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElectricityMachineBase.h"
#include "NiagaraComponent.h"
#include "OxygenGeneratorElectricity.generated.h"

class AMyCharacter;

UCLASS()
class STUCKINSPACE_API AOxygenGeneratorElectricity : public AElectricityMachineBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AOxygenGeneratorElectricity();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UNiagaraComponent* SteamEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Electricity")
	float ElectricityCost = 10.f;

	UFUNCTION(BlueprintCallable)
	bool RefillOxygen(AMyCharacter* Character);

private:
	bool ConsumeElectricity();
};
