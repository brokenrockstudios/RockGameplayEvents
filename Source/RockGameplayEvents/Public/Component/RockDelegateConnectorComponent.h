// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegate/RockGameplayEventConnection.h"
#include "RockDelegateConnectorComponent.generated.h"

UCLASS(ClassGroup="RockGameplayEvents", DisplayName="RockDelegateConnector",
	meta=(BlueprintSpawnableComponent), HideCategories = (Activation, Navigation, Tags, Cooking, AssetUserData))
class ROCKGAMEPLAYEVENTS_API URockDelegateConnectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URockDelegateConnectorComponent();

	// There is no need for this to be false at this time. Just remove the component after binding
	// As we have no longer term functionality for this Component at this time.
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Rock Delegate Connector")
	bool bAutoDestroyAfterBind = true;

	UPROPERTY(EditAnywhere, Category = "Rock Delegate Connector")
	TArray<FRockGameplayEventConnection> DelegateConnections;

	// Incoming readonly connections
	UPROPERTY(VisibleAnywhere, Category = "Rock Delegate Connector")
	TArray<FRockGameplayIncomingConnection> IncomingConnections;

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// How would you know if a connection is stale? 
	// Perhaps we are purging IncomingConnections?
	// void PurgeStaleConnections();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};


// We could creat a proxy VisualizationComponent that is only visible in the editor and is added when actor is 'targeted' and stores the stuff
// It should cease to exist in play or in deployed modes?
// This would allow us to see the connections in the editor without having to add the extra components to the actor long term
// This would be a good way to visualize the connections without cluttering the actor
// SetIsVisualizationComponent
