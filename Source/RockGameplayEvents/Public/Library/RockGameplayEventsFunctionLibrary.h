// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Misc/FindBetterHome.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RockGameplayEventsFunctionLibrary.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API URockGameplayEventsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// UFUNCTION(BlueprintCallable)
	// static void BroadcastEvent(UPARAM(ref) FRockGameplayEventDelegate& EventDelegate, const AController* EventInstigator);

	// This function is used by the FMemberReference to narrow down the type of bindable functions as the PrototypeFunction
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static void Prototype_SendController(const AController* EventInstigator) {};
};
