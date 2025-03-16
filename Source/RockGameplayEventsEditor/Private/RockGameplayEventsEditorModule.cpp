// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventsEditorModule.h"

#include "RockIncomingConnectionsTask.h"
#include "Selection.h"
#include "Async/AsyncWork.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "DetailCustomization/RockDelegateConnectionsCustomization.h"

#define LOCTEXT_NAMESPACE "FRockGameplayEventMessagesModule"

void FRockGameplayEventsEditorModule::StartupModule()
{
	RegisterPropertyTypeCustomizations();

	
	OnEditorSelectionChangedHandle = USelection::SelectionChangedEvent.AddRaw(this, &FRockGameplayEventsEditorModule::OnEditorSelectionChanged);
	
}

void FRockGameplayEventsEditorModule::ShutdownModule()
{
	UnregisterPropertyTypeCustomizations();
	
	USelection::SelectionChangedEvent.Remove(OnEditorSelectionChangedHandle);
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

void FRockGameplayEventsEditorModule::OnEditorSelectionChanged(UObject* Object)
{
	USelection* Selection = Cast<USelection>(Object);
	if (!Selection)
	{
		return;
	}

	// We only are going to support updating single actor selection for now for incoming connections.
	// Later on, we can revisit this to support multiple selections.
	UObject* selected = Selection->GetSelectedObject(0);
	if (AActor * Actor = Cast<AActor>(selected))
	{
		URockDelegateConnectorComponent* connector = Actor->FindComponentByClass<URockDelegateConnectorComponent>();
		if (connector)
		{
			FAsyncTask<FRockIncomingConnectionsTask>* BackgroundTask = new FAsyncTask<FRockIncomingConnectionsTask>(Actor, connector);
			BackgroundTask->StartBackgroundTask();
		}
	}
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRockGameplayEventsEditorModule, RockGameplayEventsEditor)
