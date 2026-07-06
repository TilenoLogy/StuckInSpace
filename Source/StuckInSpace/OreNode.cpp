// Fill out your copyright notice in the Description page of Project Settings.


#include "OreNode.h"

// Sets default values
AOreNode::AOreNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   

    Mesh =
        CreateDefaultSubobject<UStaticMeshComponent>(
            TEXT("Mesh"));

    RootComponent = Mesh;


    HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
    HealthWidget->SetupAttachment(RootComponent);

    HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidget->SetDrawAtDesiredSize(true);

    // Position it above the ore
    HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

    
    Chips = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Chips"));
    Chips->SetupAttachment(RootComponent);
    Chips->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
}

// Called when the game starts or when spawned
void AOreNode::BeginPlay()
{
	Super::BeginPlay();
    Chips->Deactivate();
    HealthWidget->SetVisibility(false);
}

// Called every frame
void AOreNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AOreNode::HideHealthWidget()
{
    HealthWidget->SetVisibility(false);
    Chips->Deactivate();
}


void AOreNode::Damage_Implementation()
{

    if (!HealthWidget->IsVisible()) {
        UE_LOG(LogTemp, Warning, TEXT("invisible"));
        HealthWidget->SetVisibility(true);
    }

    UGameplayStatics::PlaySoundAtLocation(
        this,
        HitSound,
        GetActorLocation()
    );

    Shake();

    Chips->Activate(true);

    Health--;


    UHealthBar* Widget = Cast<UHealthBar>(HealthWidget->GetUserWidgetObject());

    if (Widget)
    {
        Widget->SetHealth(Health, MaxHealth, NodeName);
    }



    UE_LOG(LogTemp, Warning, TEXT("Rock hit! Health: %d"), Health);


    GetWorldTimerManager().ClearTimer(HideWidgetTimer);

    GetWorldTimerManager().SetTimer(
        HideWidgetTimer,
        this,
        &AOreNode::HideHealthWidget,
        2.0f,
        false);


    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dropping Iron Ore"));

        if (DropClass)
        {
            GetWorld()->SpawnActor<AActor>(
                DropClass,
                GetActorLocation(),
                GetActorRotation()
            );
        }

        Destroy();
    }
}
