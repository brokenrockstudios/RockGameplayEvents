// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/DelegateFunctionInfo.h"
#include "UObject/Object.h"

#include "MiscHelperFunctions.generated.h"


ROCKGAMEPLAYEVENTS_API DECLARE_LOG_CATEGORY_EXTERN(LogRockGameplayEvents, Log, All);

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
	static TArray<FRockFunctionInfo> GetFunctionsForActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	static FString BuildFunctionParameterString(UFunction* InFunction, bool bIncludeParameterType = true, bool bIncludeParameterName = false, bool bIncludeParameterFlags = false);
	
	UFUNCTION(BlueprintCallable)
	static void AddDelegateConnectorComponent(AActor* InActor);
	
	// Internal Testing	
	UFUNCTION(BlueprintCallable)
	static void LogFunctionInfo(TArray<FRockFunctionInfo> InFunctions);
	// Internal Testing
	UFUNCTION(BlueprintCallable)
	static void LogDelegateInfo(TArray<FRockDelegateInfo> InDelegates);
};


