// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegate/RockGameplayEventConnection.h"
#include "RockDelegateConnectorComponent.generated.h"

UCLASS(ClassGroup="RockGameplayEvents", DisplayName="GameplayEventConnector",
	meta=(BlueprintSpawnableComponent), HideCategories = (Activation, Navigation, Tags, Cooking, AssetUserData))
class ROCKGAMEPLAYEVENTS_API URockDelegateConnectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URockDelegateConnectorComponent();

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bShowSparseDelegates = true;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bAutoDestroyAfterBind = true;

	UPROPERTY(EditAnywhere)
	TArray<FRockGameplayEventConnection> DelegateConnections;

	// Incoming readonly connections
	UPROPERTY(VisibleAnywhere)
	TArray<FRockGameplayIncomingConnection> IncomingConnections;

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//void PurgeStaleConnections();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};


// We could creat a proxy VisualizationComponent that is only visible in the editor and is added when actor is 'targeted' and stores the stuff
// It should cease to exist in play or in deployed modes?
// This would allow us to see the connections in the editor without having to add the extra components to the actor long term
// This would be a good way to visualize the connections without cluttering the actor
// SetIsVisualizationComponent
