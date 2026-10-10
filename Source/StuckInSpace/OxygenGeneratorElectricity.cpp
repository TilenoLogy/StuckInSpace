// Fill out your copyright notice in the Description page of Project Settings.

#include "OxygenGeneratorElectricity.h"
#include "BatteryBase.h"
#include "MyCharacter.h"
#include "NetworkManager.h"

AOxygenGeneratorElectricity::AOxygenGeneratorElectricity()
{
	SteamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SteamEffect"));
	SteamEffect->SetupAttachment(RootComponent);
	SteamEffect->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	PowerConsumption = 0.f;
}

void AOxygenGeneratorElectricity::BeginPlay()
{
	Super::BeginPlay();

	// This machine only consumes electricity when RefillOxygen is called.
	PowerConsumption = 0.f;
	if (IsValid(PowerNetwork))
	{
		PowerNetwork->RecalculateNetwork();
	}

	if (SteamEffect)
	{
		SteamEffect->Deactivate();
	}
}

bool AOxygenGeneratorElectricity::RefillOxygen(AMyCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Refill"));
	if (!Character || !Character->FindItem(1, TEXT("Ice_Cubes")))
	{
		return false;
	}

	if (!ConsumeElectricity())
	{
		return false;
	}

	Character->UseItem(TEXT("Ice_Cubes"), 1);
	Character->AddOxygen();

	if (SteamEffect)
	{
		SteamEffect->Activate(true);
	}

	return true;
}

bool AOxygenGeneratorElectricity::ConsumeElectricity()
{
	if (ElectricityCost <= 0.f)
	{
		return true;
	}

	if (!IsValid(PowerNetwork))
	{
		return false;
	}

	TArray<ABatteryBase*> Batteries;
	for (AElectricityMachineBase* Device : PowerNetwork->Devices)
	{
		if (ABatteryBase* Battery = Cast<ABatteryBase>(Device))
		{
			Batteries.Add(Battery);
		}
	}

	float AvailableEnergy = 0.f;
	for (ABatteryBase* Battery : Batteries)
	{
		AvailableEnergy += Battery->AmountOfEnergy;
	}

	if (AvailableEnergy < ElectricityCost)
	{
		return false;
	}

	float RemainingCost = ElectricityCost;
	for (ABatteryBase* Battery : Batteries)
	{
		if (RemainingCost <= 0.f)
		{
			break;
		}

		RemainingCost -= Battery->RemoveEnergy(RemainingCost);
	}

	return RemainingCost <= 0.f;
}
