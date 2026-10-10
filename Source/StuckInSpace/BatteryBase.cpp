// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryBase.h"
#include "HealthBar.h"
#include "MyCharacter.h"

// Sets default values
ABatteryBase::ABatteryBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BatteryWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BatteryWidget"));
	BatteryWidget->SetupAttachment(RootComponent);
	BatteryWidget->SetWidgetSpace(EWidgetSpace::Screen);
	BatteryWidget->SetDrawAtDesiredSize(true);
}

// Called when the game starts or when spawned
void ABatteryBase::BeginPlay()
{
	Super::BeginPlay();

	AMyCharacter::SnapActorToGround(this);

	if (BatteryWidget)
	{
		BatteryWidget->SetRelativeLocation(BatteryWidgetOffset);
		BatteryWidget->SetVisibility(true);

		if (BatteryWidgetClass)
		{
			BatteryWidget->SetWidgetClass(BatteryWidgetClass);
		}
	}

}

float ABatteryBase::AddEnergy(float Energy)
{
	if (Energy <= 0.f)
	{
		return 0.f;
	}

	const float EnergyAdded = FMath::Min(Energy, FMath::Max(0.f, MaxCapacity - AmountOfEnergy));
	AmountOfEnergy += EnergyAdded;
	return EnergyAdded;
}

float ABatteryBase::RemoveEnergy(float Energy)
{
	if (Energy <= 0.f)
	{
		return 0.f;
	}

	const float EnergyRemoved = FMath::Min(Energy, FMath::Max(0.f, AmountOfEnergy));
	AmountOfEnergy -= EnergyRemoved;
	return EnergyRemoved;
}

// Called every frame
void ABatteryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BatteryWidget)
	{
		UHealthBar* Widget = Cast<UHealthBar>(BatteryWidget->GetUserWidgetObject());
		if (Widget)
		{
			Widget->SetHealth(AmountOfEnergy, MaxCapacity, FName(TEXT("Battery")));
		}
	}

}

