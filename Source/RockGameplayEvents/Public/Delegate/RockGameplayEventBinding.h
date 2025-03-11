// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "RockGameplayEventBinding.generated.h"

/* 
* Generally it's useful to know 'who' triggered a thing for many gameplay systems.
* Having AActor in the signature, helps eliminate some of the default delegates and function object dropdowns.
* e.g. It could be null, or possibly but not limited to 'APawn', 'ACharacter', 'AController', 'APlayerController'
*/
// Basic event with no parameters
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRockGameplayEvent_Basic);
// Event with a single AActor parameter
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FRockGameplayEvent, const AActor*, EventInstigator);
// Events with arbitrary Object
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Object, const AActor*, EventInstigator, const UObject*, EventObject);
// Events with Actor
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Actor, const AActor*, EventInstigator, const AActor*, EventActor);
// Events with primitive parameters
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Float, const AActor*, EventInstigator, const float, EventFloat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Int, const AActor*, EventInstigator, const int32, EventInt);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Bool, const AActor*, EventInstigator, const bool, EventBool);
// Events with Structs: String, Vector, Rotator, Transform
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_String, const AActor*, EventInstigator, const FString, EventString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Vector, const AActor*, EventInstigator, const FVector, EventVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Rotator, const AActor*, EventInstigator, const FRotator, EventRotator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRockGameplayEvent_Transform, const AActor*, EventInstigator, const FTransform, EventTransform);


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
	// TODO: Switch out for direction UFunction, so that we don't have to do the FindFunctionByName
	//UPROPERTY()
	// TObjectPtr<UFunction> FunctionToBind;
	
	void BindMulticastDelegate(UObject* DelegateOwner, const FMulticastDelegateProperty* DelegateProperty) const;
	void BindBlueprintDelegate(UObject* DelegateOwner, const FDelegateProperty* DelegateProperty) const;


	bool IsValid() const;
	FString ToString() const;

	// void BindSparseDelegate(UObject* DelegateOwner, FName DelegateName, FSparseDelegate* SparseDelegate) const;

	// Do we want to add support for primitive data to pass along? 
	// We could have a 'Payload' struct that has all the data types we want to pass along?
	// This might be okay for a single parameter, but what about 'multi parameter' events?. Would we need to support Named Arguments?
	// Maybe we just pass thru some proxy Actor or something instead?
	// I've seen some people store it off as TArray<uint8> ParamData; but not sure how that'd work with multiparam?
	// Maybe we just leave this for V2 and keep it simple for now?
	
};
