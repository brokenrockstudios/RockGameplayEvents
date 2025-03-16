// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
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
	FString GetName() const;
	FString GetNameWithClass() const;
	UFunction* GetSignatureFunction() const;
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
	
	UFunction* GetSignatureFunction() const;
};