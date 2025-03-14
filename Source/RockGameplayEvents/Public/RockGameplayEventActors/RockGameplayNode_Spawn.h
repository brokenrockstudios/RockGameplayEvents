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

	UFUNCTION(BlueprintCallable, Category = "Rock Gameplay Node")
	void DestroyAllSpawnedActors(AActor* EventInstigator);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Gameplay Node")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Gameplay Node", meta = (MakeEditWidget = true))
	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Gameplay Node")
	bool bUseInstigatorAsOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Gameplay Node")
	int32 MaxSpawnCount = 15;

	UFUNCTION(BlueprintCallable, Category = "Rock Gameplay Node")
	void TriggerInput(AActor* EventInstigator); 
	
protected:
	UPROPERTY()
	TArray<AActor*> SpawnedActors;
};
