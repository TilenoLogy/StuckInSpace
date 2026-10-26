// Fill out your copyright notice in the Description page of Project Settings.

#include "OxygenGenerator.h"
#include "NiagaraComponent.h"
#include "MyCharacter.h"

AOxygenGenerator::AOxygenGenerator() {
	SteamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SteamEffect"));
	SteamEffect->SetupAttachment(RootComponent);
	SteamEffect->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
}

bool AOxygenGenerator::RefillOxygen(AMyCharacter* Character)
{
	if (!Character ||
		!Character->FindItem(1, "Coal_Chunk") ||
		!Character->FindItem(1, "Ice_Cubes"))
	{
		return false;
	}

	Character->UseItem("Coal_Chunk", 1);
	Character->UseItem("Ice_Cubes", 1);
	Character->AddOxygen();

	if (SteamEffect)
	{
		SteamEffect->Activate(true);
	}

	return true;
}

void AOxygenGenerator::BeginPlay()
{
	Super::BeginPlay();
	SteamEffect->Deactivate();
}