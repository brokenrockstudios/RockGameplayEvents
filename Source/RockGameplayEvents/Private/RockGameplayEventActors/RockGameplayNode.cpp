// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode.h"

#include "Component/RockDelegateConnectorComponent.h"

ARockGameplayNode::ARockGameplayNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GameplayConnector = CreateDefaultSubobject<URockDelegateConnectorComponent>(TEXT("GameplayConnector"));
}

void ARockGameplayNode::TriggerOutput()
{
	OnTriggered.Broadcast();
}
