// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockGameplayEventsVisualizersModule.h"

#include "UnrealEdGlobals.h"
#include "Component/RockDelegateConnectorComponent.h"
#include "ComponentVisualizers/RockDelegateConnectorVisualizer.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "FRockGameplayEventVisualizersModule"

void FRockGameplayEventsVisualizersModule::StartupModule()
{
	RegisterComponentVisualizers();
}

void FRockGameplayEventsVisualizersModule::ShutdownModule()
{
	UnregisterAllComponentVisualizers();
}

void FRockGameplayEventsVisualizersModule::RegisterComponentVisualizers()
{
	RegisterComponentVisualizer(URockDelegateConnectorComponent::StaticClass()->GetFName(), MakeShareable(new FRockDelegateConnectorVisualizer));
}

void FRockGameplayEventsVisualizersModule::RegisterComponentVisualizer(FName ComponentClassName, const TSharedPtr<FComponentVisualizer>& Visualizer)
{
	if (GUnrealEd)
	{
		GUnrealEd->RegisterComponentVisualizer(ComponentClassName, Visualizer);
	}
	RegisteredComponentClassNames.Add(ComponentClassName);
	if (Visualizer.IsValid())
	{
		Visualizer->OnRegister();
	}
}

void FRockGameplayEventsVisualizersModule::UnregisterAllComponentVisualizers()
{
	if (GUnrealEd)
	{
		for (FName ComponentClassName : RegisteredComponentClassNames)
		{
			GUnrealEd->UnregisterComponentVisualizer(ComponentClassName);
		}
	}
	RegisteredComponentClassNames.Empty();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRockGameplayEventsVisualizersModule, RockGameplayEventsVisualizers)
