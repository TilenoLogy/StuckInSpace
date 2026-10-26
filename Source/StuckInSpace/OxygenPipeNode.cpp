#include "OxygenPipeNode.h"

AOxygenPipeNode::AOxygenPipeNode()
{
	PrimaryActorTick.bCanEverTick = false;
}

FName AOxygenPipeNode::GetResourceType() const
{
	return FName(TEXT("Oxygen"));
}

float AOxygenPipeNode::GetProductionRate() const
{
	return FMath::Max(0.f, OxygenProductionRate);
}

float AOxygenPipeNode::GetConsumptionRate() const
{
	return FMath::Max(0.f, OxygenConsumptionRate);
}

float AOxygenPipeNode::GetStoredResource() const
{
	return OxygenAmount;
}

float AOxygenPipeNode::GetResourceCapacity() const
{
	return FMath::Max(0.f, OxygenCapacity);
}

float AOxygenPipeNode::AddResource(float Amount)
{
	if (Amount <= 0.f)
	{
		return 0.f;
	}

	const float Added = FMath::Min(Amount, FMath::Max(0.f, OxygenCapacity - OxygenAmount));
	OxygenAmount += Added;
	return Added;
}

float AOxygenPipeNode::RemoveResource(float Amount)
{
	if (Amount <= 0.f)
	{
		return 0.f;
	}

	const float Removed = FMath::Min(Amount, FMath::Max(0.f, OxygenAmount));
	OxygenAmount -= Removed;
	return Removed;
}
