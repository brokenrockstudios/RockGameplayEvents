// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventsEditorModule.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "DetailCustomization/RockGameplayEventDelegateConnectionsCustomization.h"

#define LOCTEXT_NAMESPACE "FRockGameplayEventMessagesModule"

void FRockGameplayEventsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//PropertyModule.RegisterCustomPropertyTypeLayout(FRockGameplayEventDelegate::StaticStruct()->GetFName(),
	// FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRockGameplayEventDelegateCustomization::MakeInstance));
	
	PropertyModule.RegisterCustomPropertyTypeLayout(FRockGameplayEventConnection::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRockGameplayEventDelegateConnectionsCustomization::MakeInstance));
}

void FRockGameplayEventsEditorModule::ShutdownModule()
{
	if (UObjectInitialized() && FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FRockGameplayEventConnection::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRockGameplayEventsEditorModule, RockGameplayEventsEditor)
