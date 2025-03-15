// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_FX.generated.h"


class UNiagaraSystem;
// A simple example of burst FX inspired by the GameplayCueNotify_BurstEffects
UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_FX : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	ARockGameplayNode_FX( const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get() );

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FX", meta = (MakeEditWidget = true))
	FTransform SpawnTransform;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rock|FX" )
	TArray<TObjectPtr<UNiagaraSystem>> ParticleSystems;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rock|FX" )
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rock|FX" )
	TSubclassOf<UCameraShakeBase> CameraShake;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rock|FX" )
	float CameraShakeInnerRadius = 512.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rock|FX" )
	float CameraShakeOuterRadius = 2048.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rock|FX" )
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;
		
	UFUNCTION( BlueprintCallable, Category = "Rock|FX" )
	void PlayFX(AActor* EventInstigator);
};
