// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RockEditorOnlyMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROCKGAMEPLAYEVENTS_API URockEditorOnlyMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	// USed with the Gameplay Node actors for debug visuals in editor only
public:
	URockEditorOnlyMeshComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	 

	
};
