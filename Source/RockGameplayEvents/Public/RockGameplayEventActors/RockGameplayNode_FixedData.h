// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"

#include "RockGameplayNode_FixedData.generated.h"

// If you want to trigger sending some arbitrary fixed data to some other receiver. Use this!
// This is a simple way to send some fixed data to some other receiver.
// Your game likely will have more complex ways to send data around, but this is a simple way to send some fixed data around.
// Either for testing or idea prototyping.
UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_FixedData : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockGameplayNode_FixedData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_String OnTriggeredString;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_Int OnTriggeredInt;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_Float OnTriggeredFloat;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_Bool OnTriggeredBool;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_Vector OnTriggeredVector;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_Rotator OnTriggeredRotator;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	FRockGameplayEvent_Transform OnTriggeredTransform;
	// UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	// FRockGameplayEvent_Actor OnTriggeredActor;
	// UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Rock|FixedData")
	// FRockGameplayEvent_Object OnTriggeredObject;

	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_String(AActor* EventInstigator);
	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_Int(AActor* EventInstigator);
	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_Float(AActor* EventInstigator);
	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_Bool(AActor* EventInstigator);
	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_Vector(AActor* EventInstigator);
	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_Rotator(AActor* EventInstigator);
	UFUNCTION(BlueprintCallable, Category = "Rock|FixedData")
	void TriggerFixedData_Transform(AActor* EventInstigator);
	
	// Perhaps instead do a TMap of Structs with various data to send to respective multicast events?
	// What about 'delay nodes'?  Is this being over engineery? Perhaps this could/should be an Abstract class? where you have to integrate the relevant data yourself somewhere?
	// I'll leave this as WIP for now. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	FString Data_String = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	int32 Data_Int = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	float Data_Float = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	bool Data_Bool = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	FVector Data_Vector = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	FRotator Data_Rotator = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|FixedData")
	FTransform Data_Transform = FTransform::Identity;
	
	
};

