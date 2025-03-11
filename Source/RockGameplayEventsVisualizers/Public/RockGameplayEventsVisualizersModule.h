// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"
#include "Modules/ModuleManager.h"

class FRockGameplayEventsVisualizersModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Register all component visualizers */
	void RegisterComponentVisualizers();
	/** Register a visualizer for a particular component class */
	void RegisterComponentVisualizer(FName ComponentClassName, const TSharedPtr<FComponentVisualizer>& Visualizer);
	/** Unregister all visualizers */
	void UnregisterAllComponentVisualizers();

private:
	/** Array of registered component visualizers, so we know what to unregister */
	TArray<FName> RegisteredComponentClassNames;
};
