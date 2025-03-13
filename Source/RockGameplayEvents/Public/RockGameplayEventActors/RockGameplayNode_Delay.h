// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_Delay.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_Delay : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockGameplayNode_Delay(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, Category = "Rock Gameplay Node", meta=(Units="Seconds"))
	float Delay = 1.0;
	
protected:
	UFUNCTION( BlueprintCallable, Category = "Rock|Delay" )
	void TriggerDelayed(AActor* EventInstigator);

	FTimerHandle TimerHandle;

public:
};
