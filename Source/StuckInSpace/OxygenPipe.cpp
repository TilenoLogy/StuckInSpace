#include "OxygenPipe.h"
#include "OxygenPipeNode.h"

bool AOxygenPipe::SetOxygenEndpoints(AOxygenPipeNode* InFirstNode, AOxygenPipeNode* InSecondNode)
{
	return SetEndpoints(InFirstNode, InSecondNode);
}
