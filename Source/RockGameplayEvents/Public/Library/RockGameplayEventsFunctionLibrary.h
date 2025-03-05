// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/RockGameplayEventDelegate.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RockGameplayEventsFunctionLibrary.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API URockGameplayEventsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void BroadcastEvent(UPARAM(ref) FRockGameplayEventDelegate& EventDelegate, const AController* EventInstigator);

	// This function is used by the FMemberReference to narrow down the type of bindable functions as the PrototypeFunction
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void Prototype_SendController(const AController* EventInstigator) {};
	
	// TODO: Make a helper function for registering new EventDelegates at runtime that works for both C++ and Blueprint
	// Similar to Multicast delegate's MulticastDelegate.AddDynamic(this, &MyActor::SomeClass)

	// TODO: Register a new EventDelegate to be used in the EventSystem
	//UFUNCTION(BlueprintCallable)
	// static void RegisterEventDelegate(UPARAM(ref) FRockGameplayEventDelegate& EventDelegate, AActor* TargetActor, TFunction<void(AController*)>&& Callback);

	// TODO: Experimental support for ambiguous structs similar to GameplayMessageSubsystem
	// UFUNCTION(BlueprintCallable, CustomThunk, Category=Messaging, meta=(CustomStructureParam="Message", AllowAbstract="false", DisplayName="Broadcast Event"))
	// void K2_BroadcastEvent(const int32& Message);
	// DECLARE_FUNCTION(execK2_BroadcastEvent);
	// static void BroadcastEventInternal(const UScriptStruct* StructType, const void* MessageBytes);
	
};
