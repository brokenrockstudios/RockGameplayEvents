// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Component/RockDelegateConnectorComponent.h"

#include "Component/RockGameplayEventWorldSubsystem.h"

URockDelegateConnectorComponent::URockDelegateConnectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URockDelegateConnectorComponent::OnRegister()
{
	Super::OnRegister();
	UE_LOG(LogTemp, Warning, TEXT("RockDelegateConnectorComponent registered"));

	AActor* Owner = GetOwner();
	const UClass* OwnerClass = Owner->GetClass();
	for (auto Connection : DelegateConnections)
	{
		Connection.Connect(Owner, OwnerClass);
	}
}

void URockDelegateConnectorComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<URockGameplayEventWorldSubsystem>()->AddComponent(this);
}

void URockDelegateConnectorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// If bAutoDestroyAfterBind is true, then it'd have already been removed
	// We only want to explicitly RemoveComponent during EndPlay if it wasn't already removed.
	if (!bAutoDestroyAfterBind)
	{
		GetWorld()->GetSubsystem<URockGameplayEventWorldSubsystem>()->RemoveComponent(this);
	}
	Super::EndPlay(EndPlayReason);
}

