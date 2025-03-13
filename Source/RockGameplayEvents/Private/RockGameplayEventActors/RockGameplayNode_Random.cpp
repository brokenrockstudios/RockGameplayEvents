// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Random.h"


// Sets default values
ARockGameplayNode_Random::ARockGameplayNode_Random(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARockGameplayNode_Random::TriggerRandomSelection(AActor* EventInstigator)
{
	if (RandomStream.GetFraction() <= TriggerPercentage)
	{
		OnTriggered.Broadcast(EventInstigator);
	}
}

void ARockGameplayNode_Random::BeginPlay()
{
	Super::BeginPlay();
	if (RandomSeed != 0)
	{
		RandomStream.Initialize(RandomSeed);
	}
	else
	{
		RandomStream.GenerateNewSeed();
	}
}

