// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_FX.h"

#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Components/BillboardComponent.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ARockGameplayNode_FX::ARockGameplayNode_FX(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_FX"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_FX::PlayFX(AActor* EventInstigator)
{
	for (UNiagaraSystem* ParticleSystem : ParticleSystems)
	{
		if (ParticleSystem)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParticleSystem, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
		}
	}

	for (auto Sound : Sounds)
	{
		if (Sound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, SpawnTransform.GetLocation());
		}
	}
	
	if (CameraShake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, SpawnTransform.GetLocation(), CameraShakeInnerRadius, CameraShakeOuterRadius);
	}

	if (ForceFeedbackEffect)
	{
		UGameplayStatics::SpawnForceFeedbackAtLocation(GetWorld(), ForceFeedbackEffect, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
	}

	TriggerOutput(EventInstigator);
}

