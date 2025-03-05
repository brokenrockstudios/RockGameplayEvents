// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Library/RockGameplayEventsFunctionLibrary.h"

#include "Delegate/RockGameplayEventDelegate.h"

inline void URockGameplayEventsFunctionLibrary::BroadcastEvent(FRockGameplayEventDelegate& EventDelegate, const AController* EventInstigator)
{
	EventDelegate.BroadcastEvent(EventInstigator);
}




// TODO: Experimental

// void URockGameplayEventsFunctionLibrary::RegisterEventDelegate(FRockGameplayEventDelegate& EventDelegate, AActor* TargetActor, TFunction<void(AController*)>&& Callback)
// {
// 	FRockGameplayEventListener EventListener;
// 	EventListener.TargetActor = TargetActor;
// 	// TODO:
// 	// EventListener.cachedFunction = MoveTemp(Callback);
// 	
// 	EventDelegate.EventListeners.Add(EventListener);
// }
//
// void URockGameplayEventsFunctionLibrary::K2_BroadcastEvent(const int32& Message)
// {
// 	// This will never be called, the exec version below will be hit instead
// 	checkNoEntry();
// }
//
// DEFINE_FUNCTION(URockGameplayEventsFunctionLibrary::execK2_BroadcastEvent)
// {
// 	Stack.MostRecentPropertyAddress = nullptr;
// 	Stack.StepCompiledIn<FStructProperty>(nullptr);
// 	void* MessagePtr = Stack.MostRecentPropertyAddress;
// 	FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
//
// 	P_FINISH;
//
// 	if (ensure((StructProp != nullptr) && (StructProp->Struct != nullptr) && (MessagePtr != nullptr)))
// 	{
// 		P_THIS->BroadcastEventInternal(StructProp->Struct, MessagePtr);
// 	}
// }