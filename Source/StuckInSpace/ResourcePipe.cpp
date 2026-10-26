#include "ResourcePipe.h"
#include "ResourcePipeNode.h"
#include "ResourcePipeNetwork.h"

AResourcePipe::AResourcePipe()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AResourcePipe::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		AResourcePipeNetwork::RebuildAllNetworksNextTick(GetWorld());
	}
}

bool AResourcePipe::SetEndpoints(AResourcePipeNode* InFirstNode, AResourcePipeNode* InSecondNode)
{
	if (!IsValid(InFirstNode) || !IsValid(InSecondNode) || InFirstNode == InSecondNode ||
		InFirstNode->GetResourceType() != InSecondNode->GetResourceType())
	{
		return false;
	}

	FirstNode = InFirstNode;
	SecondNode = InSecondNode;
	AResourcePipeNetwork::RebuildAllNetworks(GetWorld());
	return true;
}

bool AResourcePipe::HasValidEndpoints() const
{
	return IsValid(FirstNode) && IsValid(SecondNode) && FirstNode != SecondNode &&
		FirstNode->GetResourceType() == SecondNode->GetResourceType();
}
