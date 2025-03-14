// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Random.h"

#include "Components/BillboardComponent.h"


// Sets default values
ARockGameplayNode_Random::ARockGameplayNode_Random(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Random"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_Random::TriggerRandomSelection(AActor* EventInstigator)
{
	if (RandomStream.GetFraction() <= TriggerPercentage)
	{
		OnTriggered.Broadcast(EventInstigator);
	}
	else
	{
		OnTriggered_Inverted.Broadcast(EventInstigator);
	}
}

void ARockGameplayNode_Random::ResetNode(AActor* EventInstigator)
{
	Super::ResetNode(EventInstigator);
	if (RandomSeed != 0)
	{
		RandomStream.Initialize(RandomSeed);
	}
	else
	{
		RandomStream.GenerateNewSeed();
	}
}


void ARockGameplayNode_Random::BeginPlay()
{
	Super::BeginPlay();
	ResetNode(nullptr);
}
