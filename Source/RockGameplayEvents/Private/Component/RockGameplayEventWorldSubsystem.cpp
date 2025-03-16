// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Component/RockGameplayEventWorldSubsystem.h"

#include "TimerManager.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "Engine/World.h"

void URockGameplayEventWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// Alternatively, we could wait 1-2 frames after any registration is called.
	// In the event there is some 'late added' components? Since likely any level streaming, and possibly some world partition is going to break this stuff?
	// Since presumably most components will register on BeginPlay
	// We need to delay 1 frame tick, so that all the component's had a chance for their BeginPlay to be called and bindings to occur
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &URockGameplayEventWorldSubsystem::DestroyAllComponents);
}

void URockGameplayEventWorldSubsystem::AddComponent(URockDelegateConnectorComponent* Component)
{
	Components.Add(Component);
	
}

void URockGameplayEventWorldSubsystem::RemoveComponent(URockDelegateConnectorComponent* Component)
{
	Components.Remove(Component);
}

void URockGameplayEventWorldSubsystem::DestroyAllComponents()
{
	// UE_LOG(LogRockGameplayEvents, Display, TEXT("DestroyAllComponents 🤖"));
	for (int i = 0; i < Components.Num(); i++)
	{
		URockDelegateConnectorComponent* Component = Components[i];
		if (IsValid(Component) && Component->bAutoDestroyAfterBind)
		{
			Component->DestroyComponent();
		}
	}
	// We don't need to hold onto these anymore at this time.
	Components.Empty();
}
