// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FRockGameplayEventsEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** Register all property type customizations */
	void RegisterPropertyTypeCustomizations() const;
	/** Unregister all property type customizations */
	void UnregisterPropertyTypeCustomizations() const;

private:
	/** Array of registered component visualizers, so we know what to unregister */
	TArray<FName> RegisteredComponentClassNames;
	
	void OnEditorSelectionChanged(UObject* object);
	FDelegateHandle OnEditorSelectionChangedHandle;
};
