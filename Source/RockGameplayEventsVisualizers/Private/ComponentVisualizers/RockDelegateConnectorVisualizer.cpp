// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockDelegateConnectorVisualizer.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "RockDeveloperSettings.h"
#include "SceneManagement.h"

void FRockDelegateConnectorVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const URockDelegateConnectorComponent* ConnectorComponent = Cast<const URockDelegateConnectorComponent>(Component);
	if (!ConnectorComponent) { return; }
	AActor* Owner = ConnectorComponent->GetOwner();
	if (!Owner) { return; }
	
	const FBoxSphereBounds StartBounds = Owner->GetComponentsBoundingBox(true);
	const FVector Start = StartBounds.Origin;
	
	
	const bool bShowOutgoingConnections = GetDefault<URockDeveloperSettings>()->bShowOutgoingConnections;
	if (bShowOutgoingConnections)
	{
		const FColor OutgoingColor = GetDefault<URockDeveloperSettings>()->OutgoingConnectionColor;
		const float OutgoingLineThickness = GetDefault<URockDeveloperSettings>()->OutgoingConnectionThickness;
		const bool bOutgoingConnectionDashedLines = GetDefault<URockDeveloperSettings>()->bOutgoingConnectionDashedLines;
		const bool bOutgoingWorldSpace = GetDefault<URockDeveloperSettings>()->bOutgoingVisibilityUseWorldSpace;

		const ESceneDepthPriorityGroup depthPriority = bOutgoingWorldSpace ? SDPG_World : SDPG_Foreground;
		for (const FRockGameplayEventConnection& connection : ConnectorComponent->DelegateConnections)
		{
			for (const FRockGameplayEventBinding& binding : connection.Bindings)
			{
				// Sometimes we have null TargetActors
				if (!binding.TargetActor)
				{
					continue;
				}
				const FBoxSphereBounds EndBounds = binding.TargetActor->GetComponentsBoundingBox(true);
				const FVector End = EndBounds.Origin;
				// If the distance is > some threshold, draw the directional arrow like at 30% of the distance or something like that
				// Depending on the distance, we might want to draw a dashed line instead of a solid line

				// Only draw if it's a decent apart
				// float Distance = FVector::Dist(Start, End);
				// if (Distance > 500)
				// {
				// 	FVector Direction = (End - Start).GetSafeNormal();
				// 	FMatrix ArrowToWorld = FRotationMatrix::MakeFromX(Direction);
				// 	FVector MidPoint = Start + 0.35f * (End - Start);
				// 	ArrowToWorld.SetOrigin(MidPoint);
				// 	FTransform conet;
				// 	FVector ReverseDirection = (Start - End).GetSafeNormal();
				// 	conet.SetLocation(MidPoint);
				// 	conet.SetRotation(ReverseDirection.ToOrientationQuat());
				// 	DrawWireSphereCappedCone(PDI, conet, 40, 15, 6, 8, 0, OutgoingColor, SDPG_World);
				// }

				if (bOutgoingConnectionDashedLines)
				{
					DrawDashedLine(PDI, Start, End, OutgoingColor, 32 * OutgoingLineThickness, depthPriority);
				}
				else
				{
					PDI->DrawLine(Start, End, OutgoingColor, depthPriority, OutgoingLineThickness);
				}

				// This should always be foreground, as it's mostly pointless in world space
				PDI->DrawPoint(End, OutgoingColor, 3, SDPG_Foreground);
			}
		}
	}


	const bool bShowIncomingConnections = GetDefault<URockDeveloperSettings>()->bShowIncomingConnections;
	if (bShowIncomingConnections)
	{
		const float IncomingLineThickness = GetDefault<URockDeveloperSettings>()->IncomingConnectionThickness;
		const FColor IncomingColor = GetDefault<URockDeveloperSettings>()->IncomingConnectionColor;
		const bool bIncomingConnectionDashedLines = GetDefault<URockDeveloperSettings>()->bIncomingConnectionDashedLines;
		const bool bIncomingWorldSpace = GetDefault<URockDeveloperSettings>()->bIncomingVisibilityUseWorldSpace;

		for (const FRockGameplayIncomingConnection connection : ConnectorComponent->IncomingConnections)
		{
			if (!connection.SourceActor.IsValid())
			{
				continue;
			}
			const FBoxSphereBounds EndBounds = connection.SourceActor->GetComponentsBoundingBox(true);
			const FVector End = EndBounds.Origin;
			
			if (bIncomingConnectionDashedLines)
			{
				DrawDashedLine(PDI, Start, End, IncomingColor, 32 * IncomingLineThickness, bIncomingWorldSpace);
			}
			else
			{
				PDI->DrawLine(Start, End, IncomingColor, bIncomingWorldSpace, IncomingLineThickness);
			}
		}
	}
}
