// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableBase.h"
#include "ResourcePipe.generated.h"

class AResourcePipeNode;

UCLASS(Abstract)
class STUCKINSPACE_API AResourcePipe : public APlaceableBase
{
	GENERATED_BODY()

public:
	AResourcePipe();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pipe")
	AResourcePipeNode* FirstNode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pipe")
	AResourcePipeNode* SecondNode = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	virtual bool SetEndpoints(AResourcePipeNode* InFirstNode, AResourcePipeNode* InSecondNode);

	UFUNCTION(BlueprintCallable, Category = "Resource Pipe")
	bool HasValidEndpoints() const;
};
