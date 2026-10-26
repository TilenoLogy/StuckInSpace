#include "ResourcePipeNetwork.h"
#include "ResourcePipeNode.h"
#include "ResourcePipe.h"
#include "EngineUtils.h"
#include "TimerManager.h"

AResourcePipeNetwork::AResourcePipeNetwork()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AResourcePipeNetwork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TransferResource(DeltaTime);
}

void AResourcePipeNetwork::RegisterNode(AResourcePipeNode* Node)
{
	if (!IsValid(Node))
	{
		return;
	}

	if (ResourceType.IsNone())
	{
		ResourceType = Node->GetResourceType();
	}

	if (ResourceType == Node->GetResourceType())
	{
		Nodes.AddUnique(Node);
		Node->ResourceNetwork = this;
		RecalculateNetwork();
	}
}

void AResourcePipeNetwork::RecalculateNetwork()
{
	TotalProduction = 0.f;
	TotalConsumption = 0.f;
	TotalStored = 0.f;
	TotalCapacity = 0.f;

	for (AResourcePipeNode* Node : Nodes)
	{
		if (!IsValid(Node))
		{
			continue;
		}

		TotalProduction += Node->GetProductionRate();
		TotalConsumption += Node->GetConsumptionRate();
		TotalStored += Node->GetStoredResource();
		TotalCapacity += Node->GetResourceCapacity();
	}
}

void AResourcePipeNetwork::TransferResource(float DeltaTime)
{
	if (DeltaTime <= 0.f)
	{
		return;
	}

	RecalculateNetwork();
	const float NetRate = TotalProduction - TotalConsumption;
	float Amount = FMath::Abs(NetRate) * DeltaTime;

	if (NetRate > 0.f)
	{
		for (AResourcePipeNode* Node : Nodes)
		{
			if (Amount <= 0.f || !IsValid(Node))
			{
				continue;
			}
			Amount -= Node->AddResource(Amount);
		}
	}
	else if (NetRate < 0.f)
	{
		for (AResourcePipeNode* Node : Nodes)
		{
			if (Amount <= 0.f || !IsValid(Node))
			{
				continue;
			}
			Amount -= Node->RemoveResource(Amount);
		}
	}

	RecalculateNetwork();
}

void AResourcePipeNetwork::RebuildAllNetworks(UWorld* World)
{
	if (!World)
	{
		return;
	}

	TArray<AResourcePipeNode*> Nodes;
	for (TActorIterator<AResourcePipeNode> It(World); It; ++It)
	{
		if (IsValid(*It) && !(*It)->IsActorBeingDestroyed())
		{
			Nodes.Add(*It);
		}
	}

	TArray<AResourcePipe*> Pipes;
	for (TActorIterator<AResourcePipe> It(World); It; ++It)
	{
		if (IsValid(*It) && !(*It)->IsActorBeingDestroyed() && It->HasValidEndpoints())
		{
			Pipes.Add(*It);
		}
	}

	TArray<AResourcePipeNetwork*> Networks;
	for (TActorIterator<AResourcePipeNetwork> It(World); It; ++It)
	{
		if (IsValid(*It) && !(*It)->IsActorBeingDestroyed())
		{
			Networks.Add(*It);
			It->Nodes.Empty();
		}
	}

	TSet<AResourcePipeNode*> Unassigned(Nodes);
	int32 NetworkIndex = 0;
	while (Unassigned.Num() > 0)
	{
		AResourcePipeNode* Start = *Unassigned.CreateIterator();
		Unassigned.Remove(Start);

		TArray<AResourcePipeNode*> Component;
		TArray<AResourcePipeNode*> Pending{ Start };
		while (Pending.Num() > 0)
		{
			AResourcePipeNode* Current = Pending.Pop();
			Component.Add(Current);
			for (AResourcePipe* Pipe : Pipes)
			{
				if (!Pipe || (Pipe->FirstNode != Current && Pipe->SecondNode != Current))
				{
					continue;
				}

				AResourcePipeNode* Other = Pipe->FirstNode == Current ? Pipe->SecondNode : Pipe->FirstNode;
				if (IsValid(Other) && Other->GetResourceType() == Start->GetResourceType() && Unassigned.Remove(Other))
				{
					Pending.Add(Other);
				}
			}
		}

		AResourcePipeNetwork* Network = Networks.IsValidIndex(NetworkIndex)
			? Networks[NetworkIndex]
			: World->SpawnActor<AResourcePipeNetwork>();
		if (Network)
		{
			Network->Nodes = Component;
			Network->ResourceType = Start->GetResourceType();
			for (AResourcePipeNode* Node : Component)
			{
				Node->ResourceNetwork = Network;
			}
			Network->RecalculateNetwork();
		}
		++NetworkIndex;
	}

	for (int32 Index = NetworkIndex; Index < Networks.Num(); ++Index)
	{
		if (Networks[Index] && Networks[Index]->Nodes.Num() == 0)
		{
			Networks[Index]->Destroy();
		}
	}
}

void AResourcePipeNetwork::RebuildAllNetworksNextTick(UWorld* World)
{
	if (!World || World->bIsTearingDown)
	{
		return;
	}

	World->GetTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateStatic(&AResourcePipeNetwork::RebuildAllNetworks, World));
}
