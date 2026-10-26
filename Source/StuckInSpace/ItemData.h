// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Building UMETA(DisplayName = "Building"),
    Wire UMETA(DisplayName = "Wire"),
    Pickupable UMETA(DisplayName = "Pickupable")
};

/**
 * 
 */


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> PlaceableClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType TypeOfPlacable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight = 1.0f; // Default weight per item
};




