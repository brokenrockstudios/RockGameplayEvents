// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayEventListener.h"
#include "UObject/Object.h"
#include "RockGameplayEventDelegate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRockGameplayEvent_Basic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRockGameplayEvent, const AController*, EventInstigator);

// AController can be null and doesn't always have to be provided, but in many cases it can be useful to know the pawn, controller, playerstate, etc.
// But also having it in the signature, helps eliminate some of the default delegates in function object dropdowns.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Object, const AController*, EventController, const UObject*, EventObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_String, const AController*, EventController, const FString, EventString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Int, const AController*, EventController, const int32, EventInt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Float, const AController*, EventController, const float, EventFloat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Bool, const AController*, EventController, const bool, EventBool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Vector, const AController*, EventController, const FVector, EventVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Rotator, const AController*, EventController, const FRotator, EventRotator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Transform, const AController*, EventController, const FTransform, EventTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Actor, const AController*, EventController, const AActor*, EventActor);


USTRUCT(BlueprintType, Category = "RockGameplayEventMessages")
struct ROCKGAMEPLAYEVENTS_API FRockGameplayEventDelegate
{
	GENERATED_BODY()

	void BroadcastEvent(const AController* EventInstigator);

	// Used for any and all runtime subscriptions
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Rock|GameplayEvents")
	FRockGameplayEvent OnGameplayEvent;

private:
	friend class FRockGameplayEventDelegateCustomization;

	// Only used for Placed In level imposed subscriptions
	UPROPERTY(EditInstanceOnly, Category = "Rock|GameplayEvents", meta = (AllowPrivateAccess = "true"))
	TArray<FRockGameplayEventListener> EventListeners;

	// TODO:
	// Add support for arbitrary struct or perhaps a generic UObject* similar to GAS for arbitrary payloads?
	// void BroadcastMessage(const FMessageStructType& Message)
};


// TODO: Food for thought
// If/When we delete a particular actor from the level, we should also remove it from the list of subscriptions
// But not sure how? Alternatively, maybe we should add something we explicitly 'hook' into
// on the receiving end, that way we could add a way to see who is triggering a particular actor


// Random Notes

// If TargetActor was 'null' perhaps we could look at 'self' or 'this' instead during BlueprintEditor. Otherwise this is a In level instance only property.

// What if we want to support Components later?
// But we will either need to add a HelperClass/TargetObject to differentiate between Actor and Component
// Or find a way to better populate the list to include both Actor and it's components compatible functions? 

// UPROPERTY(VisibleAnywhere)
// TObjectPtr<UObject> TargetComponent; // Component
// Add ComponentName|HelperActor|HelperClass (if we want to further differentiate Actor and Component?)
