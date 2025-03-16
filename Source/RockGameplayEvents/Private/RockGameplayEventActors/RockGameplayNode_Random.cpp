// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Random.h"
#include "Engine/Texture2D.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"

ARockGameplayNode_Random::ARockGameplayNode_Random(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
