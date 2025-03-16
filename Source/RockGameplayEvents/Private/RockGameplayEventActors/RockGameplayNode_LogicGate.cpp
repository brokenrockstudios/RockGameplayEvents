// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_LogicGate.h"

#include "Components/BillboardComponent.h"

ARockGameplayNode_LogicGate::ARockGameplayNode_LogicGate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;

		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_Logic"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_LogicGate::InputA(AActor* EventInstigator, bool bNewState)
{
	bInputAState = bNewState;
	EvaluateGate(EventInstigator);
}

void ARockGameplayNode_LogicGate::InputA_On(AActor* EventInstigator)
{
	InputA(EventInstigator, true);
}

void ARockGameplayNode_LogicGate::InputA_Off(AActor* EventInstigator)
{
	InputA(EventInstigator, false);
}

void ARockGameplayNode_LogicGate::InputB(AActor* EventInstigator, bool bNewState)
{
	bInputBState = bNewState;
	EvaluateGate(EventInstigator);
}

void ARockGameplayNode_LogicGate::InputB_On(AActor* EventInstigator)
{
	InputB(EventInstigator, true);
}

void ARockGameplayNode_LogicGate::InputB_Off(AActor* EventInstigator)
{
	InputB(EventInstigator, false);
}

void ARockGameplayNode_LogicGate::ResetNode(AActor* EventInstigator)
{
	Super::ResetNode(EventInstigator);
	bInputAState = false;
	bInputBState = false;
}

void ARockGameplayNode_LogicGate::EvaluateGate(AActor* EventInstigator)
{
	// We could add things like 'filter logic'
	// If we didn't want certain event instigators to trigger the gate

	int32 ShouldTrigger = 0;
	switch (GateType)
	{
	case ERockLogicGateType::AND:
		ShouldTrigger = bInputAState && bInputBState;
		break;
	case ERockLogicGateType::OR:
		ShouldTrigger = bInputAState || bInputBState;
		break;
	case ERockLogicGateType::XOR:
		ShouldTrigger = bInputAState != bInputBState;
		break;
	case ERockLogicGateType::NOT:
		// NOT operates only on input A
		ShouldTrigger = !bInputAState;
		break;
	case ERockLogicGateType::NAND:
		ShouldTrigger = !(bInputAState && bInputBState);
		break;
	case ERockLogicGateType::NOR:
		ShouldTrigger = !(bInputAState || bInputBState);
		break;
	case ERockLogicGateType::XNOR:
		ShouldTrigger = !(bInputAState != bInputBState);
		break;
	default:
		break;
	}

	if (ShouldTrigger != 0)
	{
		OnTrueOutput.Broadcast(EventInstigator);
	}
	else
	{
		OnFalseOutput.Broadcast(EventInstigator);
	}
	if (ShouldTrigger != 0)
	{
		TriggerOutput(EventInstigator);
	}
}
