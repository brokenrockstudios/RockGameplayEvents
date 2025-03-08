// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "ComponentVisualizer.h"
#include "CoreMinimal.h"
#include "CoreMinimal.h"

class FPrimitiveDrawInterface;
class FSceneView;
class UActorComponent;

struct ROCKGAMEPLAYEVENTSVISUALIZERS_API FRockDelegateConnectorVisualizer: public FComponentVisualizer
{
public:
	// ~Begin FComponentVisualizer Interface
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	// ~End FComponentVisualizer Interface
};


