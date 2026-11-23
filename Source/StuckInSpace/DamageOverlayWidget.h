// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageOverlayWidget.generated.h"

class UImage;

UCLASS()
class STUCKINSPACE_API UDamageOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Damage Overlay")
	void SetDamageOpacity(float Opacity);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* DamageImage;
};
