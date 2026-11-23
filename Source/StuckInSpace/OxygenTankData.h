// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OxygenTankData.generated.h"

USTRUCT(BlueprintType)
struct FOxygenTankData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TankID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OxygenCapacity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainRate = 0.0f;
};
