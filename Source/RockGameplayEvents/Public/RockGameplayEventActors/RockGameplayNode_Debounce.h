// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_Debounce.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_Debounce : public ARockGameplayNode
{
	GENERATED_BODY()
public:
	ARockGameplayNode_Debounce(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Cooldown period in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Debounce")
	float CooldownPeriod = 1.0f;
	// Should we queue inputs during the cooldown period?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Debounce")
	bool bQueueInputDuringCooldown = false;

	UFUNCTION(BlueprintCallable, Category = "Rock|Debounce")
	void TriggerInput(AActor* EventInstigator);

	UFUNCTION(BlueprintCallable, Category = "Rock|Debounce")
	bool IsCoolingDown() const;
	
	virtual void ResetNode(AActor* EventInstigator) override;

private:
	bool bIsInCooldown = false;
	FTimerHandle CooldownTimerHandle;
	int QueuedInputCount = 0;
	void OnCooldownEnd();
};
