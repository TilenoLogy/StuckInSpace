// Fill out your copyright notice in the Description page of Project Settings.


#include "OreSpawnOutside.h"
#include "OreNode.h"
#include "GameFramework/Character.h"

// Sets default values
AOreSpawnOutside::AOreSpawnOutside()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
}

// Called when the game starts or when spawned
void AOreSpawnOutside::BeginPlay()
{
	Super::BeginPlay();
	SpawnOre();
}

// Called every frame
void AOreSpawnOutside::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AOreSpawnOutside::SpawnCall() {
	GetWorldTimerManager().SetTimer(
		RespawnTimer,
		this,
		&AOreSpawnOutside::SpawnOre,
		2.0f,
		false);
}

void AOreSpawnOutside::SpawnOre() {
	UE_LOG(LogTemp, Warning, TEXT("TrySpawning"));
	TArray<AActor*> Actors;
	CollisionBox->GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		// Skip actors that are physically too far away. This prevents false overlaps 
		// from actors (like other ores) that might have huge bounding boxes due to particles/widgets.
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) > 150.0f)
		{
			continue;
		}

		// Let's only block if it's explicitly one of these types. We don't want to block on landscape!
		if (Actor->IsA<AOreNode>() ||
			Actor->ActorHasTag("Pickup") ||
			Actor->IsA<ACharacter>())
		{
			UE_LOG(LogTemp, Warning, TEXT("CantSpawn: %s"), *Actor->GetName());

			if (Actor->IsA<AOreNode>())
			{
				// If we found a manually placed OreNode close by that doesn't have a SpawnPoint, claim it!
				AOreNode* HitNode = Cast<AOreNode>(Actor);
				if (HitNode && HitNode->SpawnPoint == nullptr)
				{
					HitNode->SpawnPoint = this;
				}
			}

			SpawnCall();
			return;
		}
	}

	
	AOreNode* NewOre = GetWorld()->SpawnActorDeferred<AOreNode>(
		Ore,
		FTransform(GetActorRotation(), GetActorLocation())
	);

	if (NewOre) {
		NewOre->SpawnPoint = this;
		UGameplayStatics::FinishSpawningActor(NewOre, FTransform(GetActorRotation(), GetActorLocation()));
	}
	
	


	
}

