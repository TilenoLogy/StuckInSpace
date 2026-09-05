// Fill out your copyright notice in the Description page of Project Settings.

#include "OxygenGenerator.h"
#include "NiagaraComponent.h"

AOxygenGenerator::AOxygenGenerator() {
	SteamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SteamEffect"));
	SteamEffect->SetupAttachment(RootComponent);
	SteamEffect->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
}

void AOxygenGenerator::BeginPlay()
{
	Super::BeginPlay();
	SteamEffect->Deactivate();
}