// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventDelegate.h"
#include "Delegate/RockGameplayEventDelegateData.h"

void FRockGameplayEventDelegate::BroadcastEvent(const AController* EventInstigator)
{
	for (FRockGameplayEventDelegateData& EventData : EventSubscriptions)
	{
		EventData.BroadcastEvent(EventInstigator);
	}
}
