// Fill out your copyright notice in the Description page of Project Settings.


#include "SnappableBase.h"

// Sets default values
ASnappableBase::ASnappableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConnectionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ConnectionPoint"));
	ConnectionPoint->SetupAttachment(RootComponent);
	ConnectionPoint->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

}

// Called when the game starts or when spawned
void ASnappableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnappableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

