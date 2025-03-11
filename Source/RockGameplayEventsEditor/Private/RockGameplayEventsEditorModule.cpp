// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventsEditorModule.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "DetailCustomization/RockDelegateConnectionsCustomization.h"

#define LOCTEXT_NAMESPACE "FRockGameplayEventMessagesModule"

void FRockGameplayEventsEditorModule::StartupModule()
{
	RegisterPropertyTypeCustomizations();
}

void FRockGameplayEventsEditorModule::ShutdownModule()
{
	UnregisterPropertyTypeCustomizations();
}

void FRockGameplayEventsEditorModule::RegisterPropertyTypeCustomizations() const
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FRockGameplayEventConnection::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRockDelegateConnectionsCustomization::MakeInstance));
}

void FRockGameplayEventsEditorModule::UnregisterPropertyTypeCustomizations() const
{
	if (UObjectInitialized() && FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FRockGameplayEventConnection::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRockGameplayEventsEditorModule, RockGameplayEventsEditor)
