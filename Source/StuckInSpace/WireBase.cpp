// Fill out your copyright notice in the Description page of Project Settings.


#include "WireBase.h"
#include "Components/PrimitiveComponent.h"
#include "ElectricityMachineBase.h"
#include "NetworkManager.h"

// Sets default values
AWireBase::AWireBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWireBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* Component : PrimitiveComponents)
	{
		if (!Component)
		{
			continue;
		}

		Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Component->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
}

// Called every frame
void AWireBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWireBase::SetEndpoints(AElectricityMachineBase* InFirstMachine,
	AElectricityMachineBase* InSecondMachine)
{
	FirstMachine = InFirstMachine;
	SecondMachine = InSecondMachine;
	ANetworkManager::RebuildAllNetworks(GetWorld());
}

void AWireBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		ANetworkManager::RebuildAllNetworksNextTick(GetWorld());
	}
}

