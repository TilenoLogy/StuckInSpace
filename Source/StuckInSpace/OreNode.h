// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "HealthBar.h"
#include "BreakInterface.h"
#include "OreNode.generated.h"


class AOreSpawnOutside;

UCLASS()
class STUCKINSPACE_API AOreNode : public AActor, public IBreakInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOreNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle HideWidgetTimer;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Damage_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	AOreSpawnOutside* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ore")
	int32 Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ore")
	int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ore")
	FName NodeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ore")
	int32 ToughnessLevel = 1;

	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Drops")
	TSubclassOf<AActor> DropClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UNiagaraComponent* Chips;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HealthWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;


	UFUNCTION(BlueprintCallable, Category="Ore")
	void HideHealthWidget();


	UFUNCTION(BlueprintImplementableEvent)
	void Shake();
};
