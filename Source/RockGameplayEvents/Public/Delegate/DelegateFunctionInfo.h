// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockDelegateType.h"
#include "DelegateFunctionInfo.generated.h"

USTRUCT(Blueprintable)
struct ROCKGAMEPLAYEVENTS_API FRockDelegateInfo
{
	GENERATED_BODY()

	FString Name;
	UClass* DefiningClass;
	TObjectPtr<UFunction> SignatureFunction;
	ERockDelegateType DelegateType;
	// bool bIsMulticast;
	// bool bIsSparse;
	// Should I add the DelegateOwner, or we can just infer it elsewhere
	FString GetNameWithClass() const;
	FString GetDisplayName() const;
	FString GetSignatureFunctionString() const;
	FString GetDelegateTypeString() const;
	FString GetName() const;
};

USTRUCT(Blueprintable)
struct ROCKGAMEPLAYEVENTS_API FRockFunctionInfo
{
	GENERATED_BODY()

	FString Name;
	UClass* DefiningClass;
	EFunctionFlags Flags;
	bool bIsEvent;
	bool bIsNative;
	bool bIsRPC;
	TObjectPtr<UFunction> Function;
	
	FString GetSignatureFunctionString() const;
};