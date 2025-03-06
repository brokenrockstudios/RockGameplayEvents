// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Component/RockDelegateConnectorComponent.h"

void FRockGameplayEventConnection::Connect(const UClass* SourceClass)
{
	const FMulticastDelegateProperty* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(SourceClass, DelegatePropertyName);
	if (DelegateProperty)
	{
		for (FRockGameplayEventBinding connection : Bindings)
		{
			if (connection.TargetActor && connection.TargetActor->GetClass()->FindFunctionByName(connection.FunctionNameToBind))
			{
				FScriptDelegate Delegate;
				Delegate.BindUFunction(connection.TargetActor, connection.FunctionNameToBind);
				DelegateProperty->AddDelegate(MoveTemp(Delegate), connection.TargetActor);
			}
		}
	}
}

URockDelegateConnectorComponent::URockDelegateConnectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URockDelegateConnectorComponent::BeginPlay()
{
	// DO NOT call Super::BeginPlay();
	// because we want to DestroyComponent immediately and the engine doesn't generally like BeginPlay and EndComponent together 
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	const UClass* OwnerClass = Owner->GetClass();
	if (!OwnerClass)
	{
		return;
	}

	for (auto Connection : DelegateConnections)
	{
		Connection.Connect(OwnerClass);
	}

	// This component was only to aid in setting up delegates bindings in the level editor.
	DestroyComponent();
}
