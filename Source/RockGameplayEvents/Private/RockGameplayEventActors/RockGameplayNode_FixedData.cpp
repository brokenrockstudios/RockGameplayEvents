// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventActors/RockGameplayNode_FixedData.h"

#include "Components/BillboardComponent.h"

ARockGameplayNode_FixedData::ARockGameplayNode_FixedData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;

		FConstructorStatics(): SpriteTextureObject(TEXT("/RockGameplayEvents/Bubble_FixedData"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	EditorOnly_Sprite->Sprite = ConstructorStatics.SpriteTextureObject.Get();
#endif
}

void ARockGameplayNode_FixedData::TriggerFixedData_String(AActor* EventInstigator)
{
	OnTriggeredString.Broadcast(EventInstigator, Data_String);
	TriggerOutput(EventInstigator);
}

void ARockGameplayNode_FixedData::TriggerFixedData_Int(AActor* EventInstigator)
{
	OnTriggeredInt.Broadcast(EventInstigator, Data_Int);
	TriggerOutput(EventInstigator);
}

void ARockGameplayNode_FixedData::TriggerFixedData_Float(AActor* EventInstigator)
{
	OnTriggeredFloat.Broadcast(EventInstigator, Data_Float);
	TriggerOutput(EventInstigator);
}

void ARockGameplayNode_FixedData::TriggerFixedData_Bool(AActor* EventInstigator)
{
	OnTriggeredBool.Broadcast(EventInstigator, Data_Bool);
	TriggerOutput(EventInstigator);
}

void ARockGameplayNode_FixedData::TriggerFixedData_Vector(AActor* EventInstigator)
{
	OnTriggeredVector.Broadcast(EventInstigator, Data_Vector);
	TriggerOutput(EventInstigator);
}

void ARockGameplayNode_FixedData::TriggerFixedData_Rotator(AActor* EventInstigator)
{
	OnTriggeredRotator.Broadcast(EventInstigator, Data_Rotator);
	TriggerOutput(EventInstigator);
}

void ARockGameplayNode_FixedData::TriggerFixedData_Transform(AActor* EventInstigator)
{
	OnTriggeredTransform.Broadcast(EventInstigator, Data_Transform);
	TriggerOutput(EventInstigator);
}
