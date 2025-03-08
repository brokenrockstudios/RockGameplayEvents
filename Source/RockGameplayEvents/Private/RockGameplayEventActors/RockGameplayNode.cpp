// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode.h"

#include "Component/RockDelegateConnectorComponent.h"

ARockGameplayNode::ARockGameplayNode()
{
	PrimaryActorTick.bCanEverTick = true;
	GameplayConnector = CreateDefaultSubobject<URockDelegateConnectorComponent>(TEXT("DelegateConnector"));
}

void ARockGameplayNode::TriggerOutput()
{
	OnTriggered.Broadcast();
}
