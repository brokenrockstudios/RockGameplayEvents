// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Compare.h"
#include "Engine/Texture2D.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"

ARockGameplayNode_Compare::ARockGameplayNode_Compare(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;

		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Compare"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_Compare::InputA(AActor* EventInstigator, FString bNewState)
{
	StateA = bNewState;
	Evaluate(EventInstigator);
}

void ARockGameplayNode_Compare::InputB(AActor* EventInstigator, FString bNewState)
{
	StateB = bNewState;
	Evaluate(EventInstigator);
}

void ARockGameplayNode_Compare::ResetNode(AActor* EventInstigator)
{
	Super::ResetNode(EventInstigator);
	StateA = "";
	StateB = "";
}

void ARockGameplayNode_Compare::Evaluate(AActor* EventInstigator)
{
	if (StateA == StateB)
	{
		OnEqualOutput.Broadcast(this);
	}

	// Numerical comparisons
	if (StateA.IsNumeric() && StateB.IsNumeric())
	{
		float A = FCString::Atof(*StateA);
		float B = FCString::Atof(*StateB);

		// Compare < <= etc..
		if (A < B)
		{
			OnLessThanOutput.Broadcast(this);
		}
		else if (A > B)
		{
			OnGreaterThanOutput.Broadcast(this);
		}
		else if (A != B)
		{
			OnNotEqualOutput.Broadcast(this);
		}
		else if (A <= B)
		{
			OnLessThanOrEqualOutput.Broadcast(this);
		}
		else if (A >= B)
		{
			OnGreaterThanOrEqualOutput.Broadcast(this);
		}
	}
	// String comparisons
	else
	{
		if (StateA < StateB)
		{
			OnLessThanOutput.Broadcast(this);
		}
		else if (StateA > StateB)
		{
			OnGreaterThanOutput.Broadcast(this);
		}
		else if (StateA != StateB)
		{
			OnNotEqualOutput.Broadcast(this);
		}
		else if (StateA <= StateB)
		{
			OnLessThanOrEqualOutput.Broadcast(this);
		}
		else if (StateA >= StateB)
		{
			OnGreaterThanOrEqualOutput.Broadcast(this);
		}
	}
	TriggerOutput(EventInstigator);
}
