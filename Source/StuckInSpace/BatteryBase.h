// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElectricityMachineBase.h"
#include "Components/WidgetComponent.h"
#include "BatteryBase.generated.h"

class UUserWidget;

UCLASS()
class STUCKINSPACE_API ABatteryBase : public AElectricityMachineBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABatteryBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	

	UPROPERTY(EditAnywhere, Category="Battery")
	float MaxCapacity = 100.f;

	UPROPERTY(EditAnywhere, Category = "Battery")
	float AmountOfEnergy = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battery|UI")
	UWidgetComponent* BatteryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battery|UI")
	TSubclassOf<UUserWidget> BatteryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battery|UI")
	FVector BatteryWidgetOffset = FVector(0.f, 0.f, 150.f);

	UFUNCTION(BlueprintCallable, Category = "Battery")
	float AddEnergy(float Energy);

	UFUNCTION(BlueprintCallable, Category = "Battery")
	float RemoveEnergy(float Energy);

};
