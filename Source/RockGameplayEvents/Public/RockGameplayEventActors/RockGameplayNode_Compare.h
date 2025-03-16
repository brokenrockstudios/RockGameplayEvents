// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_Compare.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_Compare : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockGameplayNode_Compare(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Will trigger on any inputs
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|Compare")
	FRockGameplayEvent OnLessThanOutput;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|Compare")
	FRockGameplayEvent OnLessThanOrEqualOutput;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|Compare")
	FRockGameplayEvent OnEqualOutput;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|Compare")
	FRockGameplayEvent OnNotEqualOutput;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|Compare")
	FRockGameplayEvent OnGreaterThanOutput;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|Compare")
	FRockGameplayEvent OnGreaterThanOrEqualOutput;
protected:
	UFUNCTION(BlueprintCallable, Category = "Rock|Compare")
	void InputA(AActor* EventInstigator, FString bNewState);
	UFUNCTION(BlueprintCallable, Category = "Rock|Compare")
	void InputB(AActor* EventInstigator, FString bNewState);
	// Reset the gate's state
	
	virtual void ResetNode(AActor* EventInstigator) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Rock|Compare")
	FString StateA = "0";
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Rock|Compare")
   	FString StateB = "0";
	
	void Evaluate(AActor* EventInstigator);
public:
};
