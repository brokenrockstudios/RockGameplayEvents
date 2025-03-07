// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Debounce.h"

ARockGameplayNode_Debounce::ARockGameplayNode_Debounce()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CooldownPeriod = 1.0f;
	bQueueInputDuringCooldown = false;
	bIsInCooldown = false;
	QueuedInputCount = 0;
}

void ARockGameplayNode_Debounce::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ResetNode();
}

void ARockGameplayNode_Debounce::TriggerInput()
{
	if (!bIsInCooldown)
	{
		// Not in cooldown, trigger output and start cooldown
		TriggerOutput();
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

void ARockGameplayNode_Debounce::ResetNode()
{
	bIsInCooldown = false;
	QueuedInputCount = 0;
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void ARockGameplayNode_Debounce::OnCooldownEnd()
{
	bIsInCooldown = false;

	if (QueuedInputCount > 0)
	{
		// Trigger the queued inputs
		TriggerOutput();
		
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

