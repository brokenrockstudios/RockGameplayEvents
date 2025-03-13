// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockGameplayNode.h"
#include "RockGameplayNode_LogicGate.generated.h"

// Enum for the different gate types
UENUM(BlueprintType)
enum class ERockLogicGateType : uint8
{
	// AND: True when A and B are true => (1,1)
	AND     UMETA(DisplayName = "AND"),
	// OR: True when A or B are true => (1,0), (0,1), (1,1)
	OR      UMETA(DisplayName = "OR"),
	// Exclusive-OR: True when A and B are different => (0,1), (1,0)
	XOR     UMETA(DisplayName = "XOR"),

	// Not: True when A is false. B is ignored => (0,*)
	NOT     UMETA(DisplayName = "NOT"),

	// True only when A is true. B is ignored. 
	// Buffer  UMETA(DisplayName = "Buffer"),
	
	// Not-AND: True in every scenario except when A and B are true => (0,0), (1,0), (0,1)
	NAND    UMETA(DisplayName = "NAND"),
	// Not-OR: True when A and B are false => (0,0)
	NOR     UMETA(DisplayName = "NOR"),
	// Exclusive-NOR True when A and B are the same value => (0,0),(1,1)
	XNOR    UMETA(DisplayName = "XNOR"),
	
	MAX     UMETA(DisplayName = "MAX", Hidden)
};

/*
 * OnTriggered is called only when the gate is evaluated to be true.
 * OnOutputChanged is called when this node receives any input. And contains the current output state value.
 */
UCLASS()
class ROCKGAMEPLAYEVENTS_API ARockGameplayNode_LogicGate : public ARockGameplayNode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockGameplayNode_LogicGate(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Logic Gate")
	ERockLogicGateType GateType = ERockLogicGateType::AND;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock|Logic Gate")
	bool bResetOnTrigger = false;

	// Will trigger on any inputs
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, EditAnywhere, Category = "Rock|Logic Gate")
	FRockGameplayEvent OnTrueOutput;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, EditAnywhere, Category = "Rock|Logic Gate")
	FRockGameplayEvent OnFalseOutput;
	
	// Input A event
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void InputA(bool bNewState);
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void InputA_On();
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void InputA_Off();

	// Input B event
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void InputB(bool bNewState);
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void InputB_On();
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void InputB_Off();

	// Reset the gate's state
	UFUNCTION(BlueprintCallable, Category = "Rock|Logic Gate")
	void ResetGate();

private:
	// Track the state of each input
	bool bInputAState = false;
	bool bInputBState = false;

	// Evaluate the gate logic based on current inputs
	void EvaluateGate();
};
