// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventDelegate.h"
#include "Delegate/RockGameplayEventListener.h"

void FRockGameplayEventDelegate::BroadcastEvent(const AController* EventInstigator)
{
	for (FRockGameplayEventListener& EventListener : EventListeners)
	{
		EventListener.BroadcastEvent(EventInstigator);
	}
}
