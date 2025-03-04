// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventsEditorModule.h"

#include "Delegate/RockGameplayEventListener.h"
#include "DelegateCustomization/RockEventDelegateCustomization.h"

#define LOCTEXT_NAMESPACE "FRockGameplayEventMessagesModule"

void FRockGameplayEventsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(FRockGameplayEventListener::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRockEventDelegateDetails::MakeInstance));
}

void FRockGameplayEventsEditorModule::ShutdownModule()
{
	if (UObjectInitialized() && FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FRockGameplayEventListener::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRockGameplayEventsEditorModule, RockGameplayEventsEditor)
