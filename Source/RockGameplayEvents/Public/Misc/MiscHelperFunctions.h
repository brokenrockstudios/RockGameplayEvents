// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Delegate/DelegateFunctionInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MiscHelperFunctions.generated.h"

ROCKGAMEPLAYEVENTS_API DECLARE_LOG_CATEGORY_EXTERN(LogRockGameplayEvents, Log, All);

UCLASS()
class ROCKGAMEPLAYEVENTS_API UMiscHelperFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static TArray<FRockDelegateInfo> GetDelegatesForActorClass(const UClass* ActorClass);

	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static TArray<FRockDelegateInfo> GetDelegatesForActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static TArray<FRockFunctionInfo> GetFunctionsForActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static FString BuildFunctionParameterString(
		UFunction* InFunction, bool bIncludeParameterType = true, bool bIncludeParameterName = false, bool bIncludeParameterFlags = false);

	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static void AddDelegateConnectorComponent(AActor* InActor);

	// Internal Testing	
	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static void LogFunctionInfo(TArray<FRockFunctionInfo> InFunctions);
	// Internal Testing
	UFUNCTION(BlueprintCallable, Category = "RockGameplayEvents")
	static void LogDelegateInfo(TArray<FRockDelegateInfo> InDelegates);
};
