// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableOre.h"

// Sets default values
APickableOre::APickableOre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh =
		CreateDefaultSubobject<UStaticMeshComponent>(
			TEXT("Mesh"));

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void APickableOre::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableOre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableOre::Interact_Implementation() {
	AMyCharacter* Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Character && !ItemID.IsNone())
	{
		// Use the character's inventory entry point so weight and item-table
		// handling are consistent with miner collection and other pickups.
		Character->AddItem(ItemID, 1);
		UE_LOG(LogTemp, Log, TEXT("Collected pickup %s"), *ItemID.ToString());
		this->Destroy();
	}
	else if (ItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot collect %s: ItemID is None"), *GetName());
	}
}