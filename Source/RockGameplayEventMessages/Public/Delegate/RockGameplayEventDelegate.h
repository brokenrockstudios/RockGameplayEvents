// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayEventDelegateData.h"
#include "UObject/Object.h"
#include "RockGameplayEventDelegate.generated.h"

USTRUCT(BlueprintType, Category = "RockGameplayEventMessages")
struct ROCKGAMEPLAYEVENTMESSAGES_API FRockGameplayEventDelegate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FRockGameplayEventDelegateData> EventSubscriptions;
	
	void BroadcastEvent(const AController* EventInstigator);

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
