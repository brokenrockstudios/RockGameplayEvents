// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "Misc/FindBetterHome.h"
#include "RockGameplayNode_FixedData.generated.h"


// If you want to trigger sending some arbitrary fixed data to some other receiver. Use this!

// WORK IN PROGRESS
UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_FixedData : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockGameplayNode_FixedData();

	UPROPERTY()
	FRockGameplayEvent_String OnTriggeredString;
	UPROPERTY()
	FRockGameplayEvent_Int OnTriggeredInt;
	UPROPERTY()
	FRockGameplayEvent_Float OnTriggeredFloat;
	UPROPERTY()
	FRockGameplayEvent_Bool OnTriggeredBool;
	UPROPERTY()
	FRockGameplayEvent_Vector OnTriggeredVector;
	UPROPERTY()
	FRockGameplayEvent_Rotator OnTriggeredRotator;
	UPROPERTY()
	FRockGameplayEvent_Transform OnTriggeredTransform;
	UPROPERTY()
	FRockGameplayEvent_Actor OnTriggeredActor;
	UPROPERTY()
	FRockGameplayEvent_Object OnTriggeredObject;

	// Perhaps instead do a TMap of Structs with various data to send to respective multicast events?
	// What about 'delay nodes'?  Is this being over engineery? Perhaps this could/should be an Abstract class? where you have to integrate the relevant data yourself somewhere?
	// I'll leave this as WIP for now. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Data")
	FString Data;
};

