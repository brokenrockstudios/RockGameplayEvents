// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Library/RockGameplayEventsFunctionLibrary.h"

#include "Delegate/RockGameplayEventDelegate.h"

inline void URockGameplayEventsFunctionLibrary::BroadcastEvent(FRockGameplayEventDelegate& EventDelegate, const AController* EventInstigator)
{
	EventDelegate.BroadcastEvent(EventInstigator);
}

