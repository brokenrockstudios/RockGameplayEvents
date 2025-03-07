// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/DelegateFunctionInfo.h"
#include "UObject/Object.h"

#include "MiscHelperFunctions.generated.h"

/**
 *
 */
UCLASS()
class ROCKGAMEPLAYEVENTS_API UMiscHelperFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<FRockDelegateInfo> GetDelegatesForActorClass(const UClass* ActorClass);

	UFUNCTION(BlueprintCallable)
	static TArray<FRockDelegateInfo> GetDelegatesForActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	static void PrintDelegateInfo(TArray<FRockDelegateInfo> InDelegates);

	UFUNCTION(BlueprintCallable)
	static TArray<FRockFunctionInfo> GetFunctionsForActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	static void PrintFunctionInfo(TArray<FRockFunctionInfo> InFunctions);

	UFUNCTION(BlueprintCallable)
	static FString BuildFunctionParameterString(UFunction* InFunction, bool bIncludeParameterType = true, bool bIncludeParameterName = false, bool bIncludeParameterFlags = false);

	UFUNCTION(BlueprintCallable)
	static bool CanFunctionBindToDelegate(UFunction* InFunction, UFunction* DelegateFunction);

	UFUNCTION(BlueprintCallable)
	static TArray<FRockFunctionInfo> GetCompatibleFunctionsForDelegate(AActor* InActor, FRockDelegateInfo DelegateProperty);
};


