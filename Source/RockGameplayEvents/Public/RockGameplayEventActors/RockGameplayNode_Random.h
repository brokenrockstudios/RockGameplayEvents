// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_Random.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_Random : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	ARockGameplayNode_Random(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * threshold for triggering
	 * 1.0 = always trigger
	 * 0.1 = 10% chance to trigger
	 * 0.0 = never trigger
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Logic Random", meta = (ClampMin = "0.0", ClampMax = "1.0", Units = "%"))
	float TriggerPercentage = 0.5f;

	// When you want to inversion of OnTrigger
	UPROPERTY(BlueprintAssignable, Category = "Rock|Events")
	FRockGameplayEvent OnTriggered_Inverted;
	
	// Seed for random number generation (0 = use system time)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Logic Random")
	int32 RandomSeed = 0;

	// Input trigger to select a random output
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Random")
	void TriggerRandomSelection(AActor* EventInstigator);

	
	virtual void ResetNode(AActor* EventInstigator = nullptr) override;
private:
	// Initialize the random stream
	virtual void BeginPlay() override;

	// The random stream
	FRandomStream RandomStream;
};
