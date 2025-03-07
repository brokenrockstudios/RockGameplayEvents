// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.


#include "Component/RockGameplayEventWorldSubsystem.h"

#include "Component/RockDelegateConnectorComponent.h"

void URockGameplayEventWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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
	for (int i = 0; i < Components.Num(); i++)
	{
		URockDelegateConnectorComponent* Component = Components[i];
		if (Component->bAutoDestroyAfterBind)
		{
			Component->DestroyComponent();
		}
	}
	// We don't need to hold onto these anymore at this time.
	Components.Empty();
}
