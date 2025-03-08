// Fill out your copyright notice in the Description page of Project Settings.


#include "RockGameplayEventActors/RockGameplayNode_FixedData.h"


// Sets default values
ARockGameplayNode_FixedData::ARockGameplayNode_FixedData()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARockGameplayNode_FixedData::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockGameplayNode_FixedData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

