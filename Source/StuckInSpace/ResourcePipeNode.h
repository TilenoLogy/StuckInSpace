// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnappableBase.h"
#include "ResourcePipeNode.generated.h"

class AResourcePipeNetwork;

UCLASS(Abstract)
class STUCKINSPACE_API AResourcePipeNode : public ASnappableBase
{
	GENERATED_BODY()

public:
	AResourcePipeNode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	virtual FName GetResourceType() const;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	virtual float GetProductionRate() const;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	virtual float GetConsumptionRate() const;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	virtual float GetStoredResource() const;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	virtual float GetResourceCapacity() const;

	virtual float AddResource(float Amount);
	virtual float RemoveResource(float Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pipe")
	AResourcePipeNetwork* ResourceNetwork = nullptr;

protected:
	bool bDestroyingConnectedPipes = false;
	void DestroyConnectedPipes();
};
