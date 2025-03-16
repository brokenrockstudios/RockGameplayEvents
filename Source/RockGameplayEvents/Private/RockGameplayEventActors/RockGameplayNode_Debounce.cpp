// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Debounce.h"

#include "Components/BillboardComponent.h"

ARockGameplayNode_Debounce::ARockGameplayNode_Debounce(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CooldownPeriod = 1.0f;
	bQueueInputDuringCooldown = false;
	bIsInCooldown = false;
	QueuedInputCount = 0;

#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;

		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Debounce"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_Debounce::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ResetNode(nullptr);
}

void ARockGameplayNode_Debounce::TriggerInput(AActor* EventInstigator)
{
	if (!bIsInCooldown)
	{
		// Not in cooldown, trigger output and start cooldown
		TriggerOutput(nullptr);
		bIsInCooldown = true;

		// Start cooldown timer
		GetWorldTimerManager().SetTimer(
			CooldownTimerHandle,
			this,
			&ARockGameplayNode_Debounce::OnCooldownEnd,
			CooldownPeriod,
			false
		);
	}
	else if (bQueueInputDuringCooldown)
	{
		// We're in cooldown but can queue an input
		QueuedInputCount++;
	}
}

void ARockGameplayNode_Debounce::OnCooldownEnd()
{
	bIsInCooldown = false;

	if (QueuedInputCount > 0)
	{
		// Trigger the queued inputs
		TriggerOutput(nullptr);

		QueuedInputCount--;

		// If there are still queued inputs, wait for the cooldown period before triggering the next one
		if (QueuedInputCount > 0)
		{
			bIsInCooldown = true;
			GetWorldTimerManager().SetTimer(
				CooldownTimerHandle,
				this,
				&ARockGameplayNode_Debounce::OnCooldownEnd,
				CooldownPeriod,
				false
			);
		}
	}
}

bool ARockGameplayNode_Debounce::IsCoolingDown() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle);
}

void ARockGameplayNode_Debounce::ResetNode(AActor* EventInstigator)
{
	bIsInCooldown = false;
	QueuedInputCount = 0;
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}
