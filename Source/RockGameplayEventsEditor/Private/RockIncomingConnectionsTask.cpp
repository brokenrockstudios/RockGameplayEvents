// Fill out your copyright notice in the Description page of Project Settings.


#include "RockIncomingConnectionsTask.h"

#include "RockDeveloperSettings.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "Delegate/RockGameplayEventConnection.h"
#include "Misc/MiscHelperFunctions.h"

void FRockIncomingConnectionsTask::DoWork()
{
	// Check if references are still valid
	if (!WeakActor.IsValid() || !WeakConnector.IsValid())
	{
		return;
	}

	TArray<FRockGameplayIncomingConnection> NewIncomingConnections;

	// If our actor has a ConnectorComponent, look at all other components in the world and see if any of them has a target actor of this one
	// If so update it's incoming connections
	// INEFFICIENT, but that's fine for now. Move this to an ASYNC task?
	// For every Component in the world
	for (TObjectIterator<URockDelegateConnectorComponent> Itr; Itr; ++Itr)
	{
		URockDelegateConnectorComponent* otherConnector = *Itr;
		if (!IsValid(otherConnector) || otherConnector == WeakConnector.Get())
		{
			continue;
		}

		AActor* otherActor = otherConnector->GetOwner();
		if (!IsValid(otherActor))
		{
			continue;
		}

		// Look at all of its Delegate Connections
		for (FRockGameplayEventConnection& connection : otherConnector->DelegateConnections)
		{
			// And all of it's bindings
			for (FRockGameplayEventBinding& binding : connection.Bindings)
			{
				if (binding.TargetActor == WeakActor.Get())
				{
					NewIncomingConnections.Add(FRockGameplayIncomingConnection{otherActor, binding.FunctionNameToBind});
				}
			}
		}
	}


	// Execute on game thread to update the connector safely
	FFunctionGraphTask::CreateAndDispatchWhenReady(
		[weakConnector = WeakConnector, weakActor = WeakActor, NewIncomingConnections]()
		{
			if (weakConnector.IsValid())
			{
				weakConnector->IncomingConnections = NewIncomingConnections;

				const bool bAutoRemoveUnusedConnectors = GetDefault<URockDeveloperSettings>()->bAutoRemoveUnusedConnectors;
				if (bAutoRemoveUnusedConnectors
					&& weakActor.IsValid()
					&& NewIncomingConnections.Num() == 0
					&& weakConnector->DelegateConnections.Num() == 0
					&& weakConnector->CreationMethod == EComponentCreationMethod::Instance)
				{
					// Log for debugging
					UE_LOG(LogRockGameplayEvents, Display, TEXT("Removing unused connector component from %s"), *weakActor->GetName());

					// Remove the component
					weakActor->RemoveInstanceComponent(weakConnector.Get());
					weakConnector->DestroyComponent();
				}
			}
		},
		TStatId(),
		nullptr,
		ENamedThreads::GameThread
	);
}
