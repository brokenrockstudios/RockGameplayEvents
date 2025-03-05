// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RockGameplayEventListener.generated.h"

USTRUCT(BlueprintType)
struct ROCKGAMEPLAYEVENTS_API FRockGameplayEventListener
{
	GENERATED_BODY()
	friend struct FRockGameplayEventDelegate;
	friend class URockGameplayEventsFunctionLibrary;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, meta=(FunctionReference, PrototypeFunction="/Script/RockGameplayEvents.RockGameplayEventsFunctionLibrary.Prototype_SendController"))
	FMemberReference EventFunctionReference;

private:
	// Micro optimization so we don't have to re-look it up everytime?
	// If you prefer saving memory, drop this and just look it up in Execute everytime?
	// If we didn't cache it, we could make Execute a const function everywhere
	// Perhaps we could 'bind' it a better way like GameplayMessageSubsystem or a Multicast Delegate does it instead?
	UPROPERTY(Transient)
	TObjectPtr<UFunction> cachedFunction = nullptr;

	// Should we store instead as a TFunction?
	// TFunction<void(AController*)>
	

	void BroadcastEvent(const AController* EventInstigator);
};
