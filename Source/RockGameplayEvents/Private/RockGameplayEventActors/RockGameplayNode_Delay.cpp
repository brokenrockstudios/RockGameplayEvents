// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Delay.h"

#include "Components/BillboardComponent.h"


// Sets default values
ARockGameplayNode_Delay::ARockGameplayNode_Delay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Delay"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_Delay::TriggerDelayed(AActor* EventInstigator)
{
	// If the timer already exists, reset it
	if (TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ARockGameplayNode_Delay::TriggerOutput, EventInstigator);
	GetWorldTimerManager().SetTimer(TimerHandle, RespawnDelegate, Delay, false);
}
