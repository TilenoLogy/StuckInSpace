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

	if (Character)
	{
		bool bAlreadyHave = false;
		for (struct FItem& temp : Character->Inventory) {
			if (temp.ItemName == ItemID) {
				temp.Amount++;
				bAlreadyHave = true;
				UE_LOG(LogTemp, Warning, TEXT("Old"));
				break;
			}
		}

		if (!bAlreadyHave) {
			struct FItem temp;
			temp.ItemName = ItemID;
			temp.Amount++;
			Character->Inventory.Add(temp);

			UE_LOG(LogTemp, Warning, TEXT("New"));
		}
		this->Destroy();
	}
}