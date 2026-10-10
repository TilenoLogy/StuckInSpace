// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakInterface.h"
#include "PlaceableBase.generated.h"

UCLASS()
class STUCKINSPACE_API APlaceableBase : public AActor, public IBreakInterface
{
	GENERATED_BODY()

public:
	APlaceableBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placeable")
	bool bCanBeBroken = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placeable")
	FName DestroyedItemID;

	virtual void Damage_Implementation() override;
};
