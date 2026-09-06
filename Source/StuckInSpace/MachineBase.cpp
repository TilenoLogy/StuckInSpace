// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineBase.h"

// Sets default values
AMachineBase::AMachineBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Box->SetupAttachment(RootComponent);
	Box->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Box->SetBoxExtent(FVector(80.0f, 80.0f, 80.0f));


	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldDynamic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->SetGenerateOverlapEvents(true);


	Box->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMachineBase::OnBoxBeginOverlap);

	Box->OnComponentEndOverlap.AddDynamic(
		this,
		&AMachineBase::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void AMachineBase::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UUserWidget>(
			GetWorld(),
			InteractionWidgetClass);

		if (InteractionWidget)
		{
			InteractionWidget->AddToViewport();
			InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Called every frame
void AMachineBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMachineBase::OnBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player entered"));
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void AMachineBase::OnBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Player left"));
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMachineBase::Damage_Implementation()
{
	
	UE_LOG(LogTemp, Warning, TEXT("destroy"));
	Destroy();
}