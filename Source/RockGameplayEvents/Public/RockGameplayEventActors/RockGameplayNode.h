// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/RockGameplayEventDelegate.h"
#include "GameFramework/Actor.h"
#include "RockGameplayNode.generated.h"

class URockDelegateConnectorComponent;

// Should these be Components?
// Or at least some of the logic be moved to BPFL or Components
// So that they can be 'composed' of in other ways?
UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rock|Gameplay")
	TObjectPtr<URockDelegateConnectorComponent> GameplayConnector = nullptr;
	
	// Sets default values for this actor's properties
	ARockGameplayNode();

	// Common output event for all logic nodes.
	// Some gameplay nodes will have specific output events with parameters as needed. 
    UPROPERTY(BlueprintAssignable, Category = "Rock|Events")
    FRockGameplayEvent_Basic OnTriggered;

	// Trigger the generic output event.
	UFUNCTION(BlueprintCallable, Category = "Rock|Events")
	virtual void TriggerOutput();
};
