// Fill out your copyright notice in the Description page of Project Settings.


#include "OreNode.h"
#include "OreSpawnOutside.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacter.h"

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


    AMyCharacter* MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (MyCharacter && ToughnessLevel <= MyCharacter->Strength) {

        Health--;

    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Unable to break"));

        // If a widget class was assigned, create a temporary widget to notify the player
        if (WidgetClass)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
            if (PC)
            {
                UUserWidget* TempWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
                if (TempWidget)
                {
                    TempWidget->AddToViewport();

                    // Remove the temporary widget after 2 seconds
                    TWeakObjectPtr<UUserWidget> WeakWidget(TempWidget);
                    FTimerDelegate TimerDel;
                    TimerDel.BindLambda([WeakWidget]() {
                        if (WeakWidget.IsValid())
                        {
                            WeakWidget->RemoveFromParent();
                        }
                    });
                    FTimerHandle TempWidgetHandle;
                    GetWorldTimerManager().SetTimer(TempWidgetHandle, TimerDel, 2.0f, false);
                }
            }
        }
    }
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
                AActor* Actor = GetWorld()->SpawnActor<AActor>(
                    DropClass,
                    GetActorLocation(),
                    GetActorRotation()
                );

                if (Actor)
                {
                    Actor->Tags.Add(FName("Pickup"));
                }
            }

            if (SpawnPoint) {
                UE_LOG(LogTemp, Warning, TEXT("ImCalling"));
                SpawnPoint->SpawnCall();
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("NoNo"));
            }

            Destroy();
        
    }
}
