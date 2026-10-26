// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourcePipeNetwork.generated.h"

class AResourcePipeNode;

UCLASS()
class STUCKINSPACE_API AResourcePipeNetwork : public AActor
{
	GENERATED_BODY()

public:
	AResourcePipeNetwork();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Resource Pipe")
	TArray<AResourcePipeNode*> Nodes;

	UPROPERTY(BlueprintReadOnly, Category = "Resource Pipe")
	FName ResourceType = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Resource Pipe")
	float TotalProduction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Resource Pipe")
	float TotalConsumption = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Resource Pipe")
	float TotalStored = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Resource Pipe")
	float TotalCapacity = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	void RegisterNode(AResourcePipeNode* Node);

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	void RecalculateNetwork();

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	static void RebuildAllNetworks(UWorld* World);

	static void RebuildAllNetworksNextTick(UWorld* World);

protected:
	void TransferResource(float DeltaTime);
};
