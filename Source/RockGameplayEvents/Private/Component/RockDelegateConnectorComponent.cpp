// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Component/RockDelegateConnectorComponent.h"

#include "Component/RockGameplayEventWorldSubsystem.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

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

	// DO NOT move to OnRegister, can end up multiple registering the same delegate, unless we were to add some kind of check
	// Otherwise, we could store if it's been registered or not, but that's just more state to manage.
	// Especially if we aren't careful about unbinding them during delete or other edge cases
	// If this were to ever become a problem, we could investigate an OnRegister (pre beginplay approach?)
	
	// TODO Move this to the Connection, because later we might want to support binding to components or arbitrary objects.
	// Which means when we are assigning it, we can just assign it then as the actor for now, makes upgrading easier later on.
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

#if WITH_EDITOR
EDataValidationResult URockDelegateConnectorComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult SuperResult = Super::IsDataValid(Context);

	for (const FRockGameplayEventConnection& Connection : DelegateConnections)
	{
		if (Connection.IsDataValid(Context) == EDataValidationResult::Invalid)
		{
			const FText NameOfComponent = FText::FromString(GetReadableName());
			Context.AddWarning(FText::Format(
				NSLOCTEXT("RockGameplayEvents", "DelegateConnectionInvalid", "[{0}] Has an invalid entry. {1}"),
				NameOfComponent, FText::FromString(Connection.GetDelegateNameString())));
			return EDataValidationResult::Invalid;
		}
	}
	return SuperResult;
}
#endif
