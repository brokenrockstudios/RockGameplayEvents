// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventDelegateData.h"

void FRockGameplayEventDelegateData::BroadcastEvent(const AController* EventInstigator)
{
	if (IsValid(TargetActor) && EventFunctionReference.GetMemberName().IsValid())
	{
		if (cachedFunction == nullptr)
		{
			cachedFunction = EventFunctionReference.ResolveMember<UFunction>(TargetActor.GetClass());
		}
		if (cachedFunction != nullptr)
		{
			struct
			{
				const AController* EventInstigator;
			} Args = { EventInstigator };

			TargetActor->ProcessEvent(cachedFunction, &Args);
		}
	}
}
