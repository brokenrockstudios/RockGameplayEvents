// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventsEditorModule.h"

#include "Delegate/RockGameplayEventDelegate.h"
#include "DetailCustomization/RockGameplayEventDelegateCustomization.h"

#define LOCTEXT_NAMESPACE "FRockGameplayEventMessagesModule"

void FRockGameplayEventsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(FRockGameplayEventDelegate::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRockGameplayEventDelegateCustomization::MakeInstance));
}

void FRockGameplayEventsEditorModule::ShutdownModule()
{
	if (UObjectInitialized() && FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FRockGameplayEventDelegate::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRockGameplayEventsEditorModule, RockGameplayEventsEditor)
