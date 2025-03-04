// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FRockGameplayEventsEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
