// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_Spawn.h"

#include "Components/BillboardComponent.h"


ARockGameplayNode_Spawn::ARockGameplayNode_Spawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;

		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Spawn"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_Spawn::DestroyAllSpawnedActors(AActor* EventInstigator)
{
	for (AActor* spawnedActor : SpawnedActors)
	{
		if (spawnedActor)
		{
			spawnedActor->Destroy();
		}
	}
	SpawnedActors.Empty();
}

void ARockGameplayNode_Spawn::TriggerInput(AActor* EventInstigator)
{
	// clean up array by checking if the actors are still valid
	for (int32 i = SpawnedActors.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(SpawnedActors[i]))
		{
			SpawnedActors.RemoveAt(i);
		}
	}

	if (SpawnedActors.Num() >= MaxSpawnCount)
	{
		// UE_LOG(LogTemp, Warning, TEXT("MaxSpawnedActors reached, cannot spawn more actors"));
		return;
	}

	// Spawn the actor
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	if (bUseInstigatorAsOwner)
	{
		SpawnParams.Owner = EventInstigator;
	}
	else
	{
		SpawnParams.Owner = this;
	}
	if (APawn* pawnInstigator = Cast<APawn>(EventInstigator))
	{
		SpawnParams.Instigator = pawnInstigator;
	}
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UE_LOG(LogTemp, Warning, TEXT("SpawnedActors.Num() = %d at Location %s"), SpawnedActors.Num(), *GetActorLocation().ToString());
	AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(
		ActorToSpawn,
		GetActorLocation() + SpawnTransform.GetLocation(),
		SpawnTransform.Rotator(),
		SpawnParams);
	if (spawnedActor)
	{
		SpawnedActors.Add(spawnedActor);
	}

	TriggerOutput(EventInstigator);
}
