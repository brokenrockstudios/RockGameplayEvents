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
}

void URockDelegateConnectorComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<URockGameplayEventWorldSubsystem>()->AddComponent(this);
	
	// DO NOT move to OnRegister, can end up multiple registering the same delegate, unless we were to add some kind of check for World or something
	// But BeginPlay should be otherwise fine?
	// Otherwise we could store if it's been registered or not, but that's just more state to manage.
	// Especially if you aren't careful about unbinding them during delete or other edge cases
	// If this were to ever become a problem, we could investigate an OnRegister (pre beginplay approach?) 
	AActor* Owner = GetOwner();
	const UClass* OwnerClass = Owner->GetClass();
	for (auto Connection : DelegateConnections)
	{
		Connection.Connect(Owner, OwnerClass);
	}
}

void URockDelegateConnectorComponent::OnUnregister()
{
	Super::OnUnregister();
	
	// for (auto Connection : DelegateConnections)
	// {
	// 	Connection.Disconnect();
	// }
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

