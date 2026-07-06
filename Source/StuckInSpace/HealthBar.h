// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class STUCKINSPACE_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()


public:

    UFUNCTION(BlueprintImplementableEvent)
    void SetHealth(float Current, float MaxHealth, FName NodeName);
};
