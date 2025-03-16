// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
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
	UPROPERTY(EditAnywhere, Category = "RockGameplayEvents")
	FName DelegatePropertyName;
	// Multicast or Singular type
	UPROPERTY(VisibleAnywhere, Category = "RockGameplayEvents")
	ERockDelegateType DelegateType = ERockDelegateType::MulticastDelegate;

	UPROPERTY(VisibleAnywhere, DisplayName = "Delegate Parameters", Category = "RockGameplayEvents")
	FString DelegateParameterList;


	//UPROPERTY(EditAnywhere)
	//UFunction* DelegateFunction;
	//UPROPERTY(EditAnywhere)
	//FMemberReference DelegateReference;

	UPROPERTY(EditAnywhere, Category = "RockGameplayEvents")
	TArray<FRockGameplayEventBinding> Bindings;

private:
	friend class URockDelegateConnectorComponent;
	void Connect(UObject* DelegateOwner, const UClass* SourceClass);

	FString ToString() const;
	FString GetDelegateNameString() const;

#if WITH_EDITOR
	EDataValidationResult IsDataValid(class FDataValidationContext& Context) const;
#endif
};

USTRUCT(BlueprintType)
struct ROCKGAMEPLAYEVENTS_API FRockGameplayIncomingConnection
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "RockGameplayEvents")
	TWeakObjectPtr<AActor> SourceActor;

	// The owner's Multicast Delegate Property to bind to
	// This technically could be a FMemberReference? But what that extra data buys us is questionable?
	UPROPERTY(VisibleAnywhere, DisplayName = "Bound to Delegate",Category = "RockGameplayEvents")
	FName DelegatePropertyName;


	//FRockConnectionHandler ConnectionHandler;

	bool operator==(const FRockGameplayIncomingConnection& other) const
	{
		return SourceActor == other.SourceActor && DelegatePropertyName == other.DelegatePropertyName;
	};
};
