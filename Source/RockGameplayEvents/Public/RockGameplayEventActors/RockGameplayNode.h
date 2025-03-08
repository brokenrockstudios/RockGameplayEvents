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


	// Enable and Disable the Gameplay Node
	UFUNCTION(BlueprintCallable, Category = "Rock|Events")
	virtual void Enable(AController* EventInstigator);

	UFUNCTION(BlueprintCallable, Category = "Rock|Events")
	virtual void Disable(AController* EventInstigator);

	UFUNCTION(BlueprintPure, Category = "Rock|Events")
	bool IsEnabled() const;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rock|Events", meta = (AllowPrivateAccess))
	bool bIsEnabled = true;
	
	// Trigger the generic output event.
	UFUNCTION(BlueprintCallable, Category = "Rock|Events")
	virtual void TriggerOutput();
};


// What other nodes can we make?

// If we do a comparison node, do we compare Int, String, Bools, or what Value?
// Bools, ints, could 'probably' be jammed into a float/double? Should we just use doubles everywhere?
// 6 different outputs callbacks? a<b a<=b a==b, a>=b a>b, a!=b
// Though we could just have a single output and have the comparison type be a parameter?

// A delay node, that waits for a certain amount of time before triggering an output
// we have a 'debounce' but delay would just delay a single input, or how would we allow queued delays working?
// Perhaps on short term, just a single delay, and then we can add a 'delay queue' later on

// a 'timer' node, that triggers an output every X seconds
// a string match node? one that requires certain inputs added to a buffer of length N?
// Some kind of 'string' lock node, that triggers an output when a certain string is inputted into it?


// a spawner node. that spawns an actor, and then triggers an output when it is destroyed?
// a 'destroy' node, that destroys an actor, and then triggers an output when it is destroyed?

// a "distance sensor" node, that triggers an output when an actor gets within a certain distance of it

// a 'collision sensor' node, that triggers an output when an actor collides with it (Can we just leverage TriggerVolumes?)

// Pressure plate node, that triggers an output when an actor is on top of it (slightly different than trigger volumes, since this would require a specific actor filter to be on top of it?)

// Daylight detector?  Can this be generalized or are people's needs too varied?

// A 'proximity sensor' node, that triggers a certain amount of actors are within it?

// a 'barrier node' that blocks actors from passing through it, and triggers an output when an actor tries to pass through it?

// a launcher node, that launches an actor in a certain direction, and triggers an output when it is launched?

// a capture area that accumulates a 'charge' when actors are within it, and triggers an output when the charge reaches a certain level?

// A commented out one that listens or broadcasts on global GameplayMessageRouter (The Lyra message subsystem)

// a destroy actor node, that destroys an actor, and triggers an output when it is destroyed?

// countdown node, that triggers an output when a countdown reaches 0? (Must be triggered so many times)
// usage node, that only allows so many triggers before having 0 uses left?






// FX
// a 'bell' or 'speaker' node,  that plays a gameplay cue
// a light node, that turns on or off a light
  // This could be expanded a lot by like 'warm up time' and other things
  // Though we could offer a 'simple version' with this plugin, and more custom ones internally, like make it 'extendable'.
// Play a particule effect
// Perhaps it should be like a basic GameplayCue type thing? that can play a variety of things? or should they be separate actors?
	// Perhaps a simple one for plugin, and more advanced ones are custom made per game
















