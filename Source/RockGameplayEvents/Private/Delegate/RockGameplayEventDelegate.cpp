// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventDelegate.h"
#include "Delegate/RockGameplayEventListener.h"

void FRockGameplayEventDelegate::BroadcastEvent(const AController* EventInstigator)
{
	for (FRockGameplayEventListener& EventListener : EventListeners)
	{
		EventListener.BroadcastEvent(EventInstigator);
		// TODO: If a given event listener is invalid, should we remove from the list?
		// Perhaps on the first time call, we just move all of these into regular delegates?
		// Or perhaps have a worldsubsystem that does this on beginplay?
	}
	
	OnGameplayEvent.Broadcast(EventInstigator);
}
