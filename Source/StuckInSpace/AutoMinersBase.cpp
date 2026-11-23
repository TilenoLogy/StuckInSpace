// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoMinersBase.h"
#include "MyCharacter.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAutoMinersBase::AAutoMinersBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinerMesh"));
	RootComponent = MinerMesh;

}

// Called when the game starts or when spawned
void AAutoMinersBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAutoMinersBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAutoMinersBase::StartMining()
{
	GetWorldTimerManager().SetTimer(
		MinerTimer,
		this,
		&AAutoMinersBase::MineOre,
		MiningInterval,
		true
	);

}

void AAutoMinersBase::StopMining()
{
	IsMining = false;
	GetWorldTimerManager().ClearTimer(MinerTimer);
}

void AAutoMinersBase::MineOre()
{
	if (StoredAmount >= StorageCapacity)
	{
		return;
	}

	StoredAmount++;
}


void AAutoMinersBase::CollectOre(AMyCharacter* Character)
{
	
	if (!Character || StoredAmount <= 0)
	{
		return;
	}

	if (OreItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("%s cannot collect ore: OreItemID is None. Set it on the placed miner instance or its Blueprint defaults."),
			*GetName());
		return;
	}

	Character->AddItem(OreItemID, StoredAmount);

	StoredAmount = 0;
}