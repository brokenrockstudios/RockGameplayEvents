// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_LogicGate.h"

ARockGameplayNode_LogicGate::ARockGameplayNode_LogicGate()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARockGameplayNode_LogicGate::InputA(bool bNewState)
{
	bInputAState = bNewState;
	EvaluateGate();
}

void ARockGameplayNode_LogicGate::InputA_On()
{
	 InputA(true);
}

void ARockGameplayNode_LogicGate::InputA_Off()
{
	InputA(false);
}

void ARockGameplayNode_LogicGate::InputB(bool bNewState)
{
	bInputBState = bNewState;
	EvaluateGate();
}

void ARockGameplayNode_LogicGate::InputB_On()
{
	InputB(true);
}

void ARockGameplayNode_LogicGate::InputB_Off()
{
	InputB(false);
}

void ARockGameplayNode_LogicGate::ResetGate()
{
	bInputAState = false;
	bInputBState = false;
}

void ARockGameplayNode_LogicGate::EvaluateGate()
{
	int32 bShouldTrigger = 0;
	switch (GateType)
	{
	case ERockLogicGateType::AND:
		bShouldTrigger = bInputAState && bInputBState;
		break;
	case ERockLogicGateType::OR:
		bShouldTrigger = bInputAState || bInputBState;
		break;
	case ERockLogicGateType::XOR:
		bShouldTrigger = bInputAState != bInputBState;
		break;
	case ERockLogicGateType::NOT:
		// NOT operates only on input A
		bShouldTrigger = !bInputAState;
		break;
	case ERockLogicGateType::NAND:
		bShouldTrigger = !(bInputAState && bInputBState);
		break;
	case ERockLogicGateType::NOR:
		bShouldTrigger = !(bInputAState || bInputBState);
		break;
	case ERockLogicGateType::XNOR:
		bShouldTrigger = !(bInputAState != bInputBState);
		break;
	default:
		break;
	}
	
	OnOutputChanged.Broadcast(bShouldTrigger);
	if (bShouldTrigger != 0)
	{
		TriggerOutput();
	}
}



