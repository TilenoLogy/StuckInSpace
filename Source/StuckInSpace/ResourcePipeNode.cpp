#include "ResourcePipeNode.h"
#include "ResourcePipeNetwork.h"
#include "ResourcePipe.h"
#include "EngineUtils.h"

AResourcePipeNode::AResourcePipeNode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AResourcePipeNode::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(ResourceNetwork) && GetWorld())
	{
		ResourceNetwork = GetWorld()->SpawnActor<AResourcePipeNetwork>();
	}

	if (IsValid(ResourceNetwork))
	{
		ResourceNetwork->RegisterNode(this);
	}
}

void AResourcePipeNode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyConnectedPipes();
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		AResourcePipeNetwork::RebuildAllNetworksNextTick(GetWorld());
	}
}

FName AResourcePipeNode::GetResourceType() const
{
	return NAME_None;
}

float AResourcePipeNode::GetProductionRate() const
{
	return 0.f;
}

float AResourcePipeNode::GetConsumptionRate() const
{
	return 0.f;
}

float AResourcePipeNode::GetStoredResource() const
{
	return 0.f;
}

float AResourcePipeNode::GetResourceCapacity() const
{
	return 0.f;
}

float AResourcePipeNode::AddResource(float Amount)
{
	return 0.f;
}

float AResourcePipeNode::RemoveResource(float Amount)
{
	return 0.f;
}

void AResourcePipeNode::DestroyConnectedPipes()
{
	if (bDestroyingConnectedPipes || !GetWorld())
	{
		return;
	}

	bDestroyingConnectedPipes = true;

	TArray<AResourcePipe*> ConnectedPipes;
	for (TActorIterator<AResourcePipe> It(GetWorld()); It; ++It)
	{
		if (IsValid(*It) && ((*It)->FirstNode == this || (*It)->SecondNode == this))
		{
			ConnectedPipes.Add(*It);
		}
	}

	for (AResourcePipe* Pipe : ConnectedPipes)
	{
		Pipe->Destroy();
	}

	bDestroyingConnectedPipes = false;
}
