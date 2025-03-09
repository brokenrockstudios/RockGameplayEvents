// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "RockDelegateConnectorVisualizer.h"

#include "Component/RockDelegateConnectorComponent.h"

// TODO: Allow user to override via developer config?

static const FColor OutgoingColor(255, 255, 255);
static const FColor IncomingColor(0, 255, 0);
//static const FColor TargetPointColor(0, 0, 255);

void FRockDelegateConnectorVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const URockDelegateConnectorComponent* ConnectorComponent = Cast<const URockDelegateConnectorComponent>(Component);
	if (!ConnectorComponent)
	{
		return;
	}
	AActor* Owner = ConnectorComponent->GetOwner();
	if (!Owner)
	{
		return;
	}
	const FVector Start = ConnectorComponent->GetOwner()->GetActorLocation();
	// Outgoing
	for (const FRockGameplayEventConnection& connection : ConnectorComponent->DelegateConnections)
	{
		for (const FRockGameplayEventBinding& binding : connection.Bindings)
		{
			// Sometimes we have null TargetActors
			if (!binding.TargetActor)
			{
				continue;
			}
			FVector End = binding.TargetActor->GetActorLocation();
			float Thickness = .5f;
			// If the distance is > some threshold, draw the directional arrow like at 30% of the distance or something like that
			// Depending on the distance, we might want to draw a dashed line instead of a solid line

			// Get Rotation from Start to End
			//const FMatrix ArrowToWorld((End-Start).Rotation(), Start);
			//CalculateLookAtMatrixFromTransform(

			// Only draw if it's a decent apart
			float Distance = FVector::Dist(Start, End);
			if (Distance > 500)
			{
				FVector Direction = (End - Start).GetSafeNormal();
				FMatrix ArrowToWorld = FRotationMatrix::MakeFromX(Direction);
				FVector MidPoint = Start + 0.35f * (End - Start);;
				ArrowToWorld.SetOrigin(MidPoint);
				FTransform conet;
				FVector ReverseDirection = (Start - End).GetSafeNormal();
				conet.SetLocation(MidPoint);
				conet.SetRotation(ReverseDirection.ToOrientationQuat());
				DrawWireSphereCappedCone(PDI, conet, 40, 15, 6, 8, 0, OutgoingColor, SDPG_World);
			}
			ESceneDepthPriorityGroup depthPriority = SDPG_World;
			bool bDashed = false;
			if (bDashed)
			{
				DrawDashedLine(PDI, Start, End, OutgoingColor, 32, depthPriority);
			}
			else
			{
				PDI->DrawLine(Start, End, OutgoingColor, depthPriority, 1);
			}

			PDI->DrawPoint(End, OutgoingColor, 3, SDPG_Foreground);
		}
	}

	for (const FRockGameplayIncomingConnection connection : ConnectorComponent->IncomingConnections)
	{
		if (!connection.SourceActor)
		{
			continue;
		}
		FVector End = connection.SourceActor->GetActorLocation();
		ESceneDepthPriorityGroup depthPriority = SDPG_World;
		PDI->DrawLine(Start, End, IncomingColor, depthPriority, 1);
	}

	// TODO: Incoming
	// How to draw a dotted line?
	// for (const FRockGameplayEventConnection& connection : ConnectorComponent->DelegateConnections)
	// {
	// 	for (const FRockGameplayEventBinding& binding : connection.Bindings)
	// 	{
	// 		FVector End = binding.TargetActor->GetActorLocation();
	// 		PDI->DrawLine(End, Start, FLinearColor::Green, SDPG_Foreground);
	// 	}
	// }
}
