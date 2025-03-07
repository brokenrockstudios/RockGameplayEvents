// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "RockGameplayEventBinding.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct ROCKGAMEPLAYEVENTS_API FRockGameplayEventBinding
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> TargetActor;

	// This must match the signature of FRockGameplayEventConnection::DelegatePropertyName
	UPROPERTY(EditAnywhere)
	FName FunctionNameToBind;

	void BindDelegate(UObject* DelegateOwner, const FMulticastDelegateProperty* DelegateProperty) const;
	void BindSparseDelegate(UObject* DelegateOwner, FName DelegateName, FSparseDelegate* SparseDelegate) const;
};
