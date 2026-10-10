// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableBase.h"
#include "WireBase.generated.h"

class AElectricityMachineBase;

UCLASS()
class STUCKINSPACE_API AWireBase : public APlaceableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWireBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Electricity")
	AElectricityMachineBase* FirstMachine = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Electricity")
	AElectricityMachineBase* SecondMachine = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Electricity")
	void SetEndpoints(AElectricityMachineBase* InFirstMachine,
		AElectricityMachineBase* InSecondMachine);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
