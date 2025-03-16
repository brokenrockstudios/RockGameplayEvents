// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RockGameplayEventWorldSubsystem.generated.h"

class URockDelegateConnectorComponent;

/**
 * 
 */
UCLASS()
class ROCKGAMEPLAYEVENTS_API URockGameplayEventWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void AddComponent(URockDelegateConnectorComponent* Component);
	void RemoveComponent(URockDelegateConnectorComponent* Component);

private:
	void DestroyAllComponents();
	
	UPROPERTY()
	TArray<TObjectPtr<URockDelegateConnectorComponent>> Components;
};
