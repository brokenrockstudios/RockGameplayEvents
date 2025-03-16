// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/DelegateFunctionInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RockEventsEditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROCKGAMEPLAYEVENTSEDITOR_API URockEventsEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//
	// UFUNCTION(BlueprintCallable)
	// static TArray<FRockDelegateInfo> GetDelegatesForActorClass(const UClass* ActorClass);
	//
	// UFUNCTION(BlueprintCallable)
	// static TArray<FRockDelegateInfo> GetDelegatesForActor(AActor* InActor);
	//
	// UFUNCTION(BlueprintCallable)
	// static TArray<FRockFunctionInfo> GetFunctionsForActor(AActor* InActor);
	//
	// UFUNCTION(BlueprintCallable)
	// static FString BuildFunctionParameterString(UFunction* InFunction, bool bIncludeParameterType = true, bool bIncludeParameterName = false, bool bIncludeParameterFlags = false);
	//
	UFUNCTION(BlueprintCallable, Category = "Rock|Function Library")
	static TArray<FRockFunctionInfo> GetCompatibleFunctionsForDelegate(AActor* InActor, FRockDelegateInfo DelegateProperty);
	
	UFUNCTION(BlueprintCallable, Category = "Rock|Function Library")
	static bool CanFunctionBindToDelegate(UFunction* InFunction, UFunction* DelegateFunction);
	
	UFUNCTION(BlueprintCallable, Category = "Rock|Function Library")
	static bool IsFunctionsCompatible(UFunction* InFunction, UFunction* DelegateFunction);
	//
	// UFUNCTION(BlueprintCallable)
	// static void AddDelegateConnectorComponent(AActor* InActor);
	//
	// // Internal Testing	
	// UFUNCTION(BlueprintCallable)
	// static void LogFunctionInfo(TArray<FRockFunctionInfo> InFunctions);
	// // Internal Testing
	// UFUNCTION(BlueprintCallable)
	// static void LogDelegateInfo(TArray<FRockDelegateInfo> InDelegates);
};
