// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "RockGameplayEventBinding.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRockGameplayEvent, const AActor*, EventInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRockGameplayEvent_Basic);

// EventInstigator might be a 'pawn', 'character', acontroller, aplayercontroller or even can be null
// But also having it in the signature, helps eliminate some of the default delegates and function object dropdowns.
// Generally it's useful to know 'who' triggered a thing. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Object, const AActor*, EventInstigator, const UObject*, EventObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_String, const AActor*, EventInstigator, const FString, EventString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Int, const AActor*, EventInstigator, const int32, EventInt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Float, const AActor*, EventInstigator, const float, EventFloat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Bool, const AActor*, EventInstigator, const bool, EventBool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Vector, const AActor*, EventInstigator, const FVector, EventVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Rotator, const AActor*, EventInstigator, const FRotator, EventRotator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Transform, const AActor*, EventInstigator, const FTransform, EventTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Actor, const AActor*, EventInstigator, const AActor*, EventActor);


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
