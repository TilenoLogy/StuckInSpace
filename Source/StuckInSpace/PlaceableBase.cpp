// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaceableBase.h"

APlaceableBase::APlaceableBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlaceableBase::Damage_Implementation()
{
	Destroy();
}
