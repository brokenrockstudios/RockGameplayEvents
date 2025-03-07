// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegate/RockGameplayEventConnection.h"
#include "RockDelegateConnectorComponent.generated.h"

UCLASS(ClassGroup=(RockGameplayEvents), meta=(BlueprintSpawnableComponent), HideCategories = (Activation, Navigation, Tags, Cooking, AssetUserData))
class ROCKGAMEPLAYEVENTS_API URockDelegateConnectorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	URockDelegateConnectorComponent();
	
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bShowSparseDelegates = true;
	
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bAutoDestroyAfterBind = true;
	
	// TODO: Wrap this in something else that can be optionally stuck on actor via a UObject instead of ActorComponent?
	UPROPERTY(EditAnywhere)
	TArray<FRockGameplayEventConnection> DelegateConnections;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRegister() override;
};
