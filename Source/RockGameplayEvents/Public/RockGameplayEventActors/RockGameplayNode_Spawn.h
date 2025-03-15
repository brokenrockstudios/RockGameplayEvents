// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_Spawn.generated.h"

UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_Spawn : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockGameplayNode_Spawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Rock|Spawn")
	void DestroyAllSpawnedActors(AActor* EventInstigator);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Spawn")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Spawn", meta = (MakeEditWidget = true))
	FTransform SpawnTransform = FTransform(FVector(0, 0, 200));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Spawn")
	bool bUseInstigatorAsOwner = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Spawn")
	int32 MaxSpawnCount = 15;

	UFUNCTION(BlueprintCallable, Category = "Rock|Spawn")
	void TriggerInput(AActor* EventInstigator);

protected:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
