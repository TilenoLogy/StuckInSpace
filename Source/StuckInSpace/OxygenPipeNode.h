// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourcePipeNode.h"
#include "OxygenPipeNode.generated.h"

UCLASS()
class STUCKINSPACE_API AOxygenPipeNode : public AResourcePipeNode
{
	GENERATED_BODY()

public:
	AOxygenPipeNode();

	virtual FName GetResourceType() const override;
	virtual float GetProductionRate() const override;
	virtual float GetConsumptionRate() const override;
	virtual float GetStoredResource() const override;
	virtual float GetResourceCapacity() const override;
	virtual float AddResource(float Amount) override;
	virtual float RemoveResource(float Amount) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	float OxygenProductionRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	float OxygenConsumptionRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	float OxygenCapacity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	float OxygenAmount = 0.f;
};
