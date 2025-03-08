// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "RockGameplayEventBinding.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRockGameplayEvent, const AController*, EventInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRockGameplayEvent_Basic);

// AController can be null and doesn't always have to be provided, but in many cases it can be useful to know the pawn, controller, playerstate, etc.
// But also having it in the signature, helps eliminate some of the default delegates in function object dropdowns.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Object, const AController*, EventInstigator, const UObject*, EventObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_String, const AController*, EventInstigator, const FString, EventString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Int, const AController*, EventInstigator, const int32, EventInt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Float, const AController*, EventInstigator, const float, EventFloat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Bool, const AController*, EventInstigator, const bool, EventBool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Vector, const AController*, EventInstigator, const FVector, EventVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Rotator, const AController*, EventInstigator, const FRotator, EventRotator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Transform, const AController*, EventInstigator, const FTransform, EventTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Actor, const AController*, EventInstigator, const AActor*, EventActor);


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
