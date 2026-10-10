// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricityMachineBase.h"
#include "EngineUtils.h"
#include "NetworkManager.h"
#include "WireBase.h"

// Sets default values
AElectricityMachineBase::AElectricityMachineBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	ConnectionPoint->SetupAttachment(Mesh);
	
	
}

void AElectricityMachineBase::Damage_Implementation()
{
	DestroyConnectedWires();
	Super::Damage_Implementation();
}

void AElectricityMachineBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyConnectedWires();
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		ANetworkManager::RebuildAllNetworksNextTick(GetWorld());
	}
}

void AElectricityMachineBase::DestroyConnectedWires()
{
	if (bDestroyingConnectedWires || !GetWorld())
	{
		return;
	}

	bDestroyingConnectedWires = true;

	TArray<AWireBase*> Wires;
	for (TActorIterator<AWireBase> It(GetWorld()); It; ++It)
	{
		if (IsValid(*It) && ((*It)->FirstMachine == this || (*It)->SecondMachine == this))
		{
			Wires.Add(*It);
		}
	}

	for (AWireBase* Wire : Wires)
	{
		Wire->Destroy();
	}

	bDestroyingConnectedWires = false;
}

bool AElectricityMachineBase::HasWireTo(const AElectricityMachineBase* OtherMachine) const
{
	if (!IsValid(OtherMachine) || OtherMachine == this || !GetWorld())
	{
		return false;
	}

	for (TActorIterator<AWireBase> It(GetWorld()); It; ++It)
	{
		const AWireBase* Wire = *It;
		if (!IsValid(Wire))
		{
			continue;
		}

		const bool bMatchesForward = Wire->FirstMachine == this && Wire->SecondMachine == OtherMachine;
		const bool bMatchesReverse = Wire->FirstMachine == OtherMachine && Wire->SecondMachine == this;
		if (bMatchesForward || bMatchesReverse)
		{
			return true;
		}
	}

	return false;
}

// Called when the game starts or when spawned
void AElectricityMachineBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PowerNetwork))
	{
		PowerNetwork->RegisterDevice(this);
		return;
	}

	

	PowerNetwork = GetWorld()->SpawnActor<ANetworkManager>();
	if (IsValid(PowerNetwork))
	{
		PowerNetwork->RegisterDevice(this);
	}
}

// Called every frame
void AElectricityMachineBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

