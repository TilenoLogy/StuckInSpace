// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourcePipe.h"
#include "OxygenPipe.generated.h"

class AOxygenPipeNode;

UCLASS()
class STUCKINSPACE_API AOxygenPipe : public AResourcePipe
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Oxygen")
	bool SetOxygenEndpoints(AOxygenPipeNode* InFirstNode, AOxygenPipeNode* InSecondNode);
};
