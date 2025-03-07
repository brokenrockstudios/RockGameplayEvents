// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockDelegateType.h"
#include "RockGameplayEventBinding.h"
#include "UObject/Object.h"
#include "RockGameplayEventConnection.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct ROCKGAMEPLAYEVENTS_API FRockGameplayEventConnection
{
	GENERATED_BODY()
	
	// The owner's Multicast Delegate Property to bind to
	UPROPERTY(EditAnywhere)
	FName DelegatePropertyName;

	ERockDelegateType DelegateType;

	UPROPERTY(EditAnywhere)
	TArray<FRockGameplayEventBinding> Bindings;

private:
	friend class URockDelegateConnectorComponent;
	void Connect(AActor* actor, const UClass* SourceClass);
};