// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "RockDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "Rock Tools"))
class ROCKGAMEPLAYEVENTSEDITOR_API URockDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
public:

	URockDeveloperSettings();
	
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Show Outgoing Connections"))
	bool bShowOutgoingConnections = true;
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Connection Color"))
	FColor OutgoingConnectionColor = FColor(255, 128, 0);
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Line Thickness", ClampMin = "0.1", ClampMax = "10.0"))
	float OutgoingConnectionThickness = 1.0f;
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Connection Dashed Lines", ToolTip = "Dashed Distance is defined by line thickness parameter"))
	bool bOutgoingConnectionDashedLines = false;
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Connections Use World Space", ToolTip = "If true, the connection lines will be in world space, otherwise they will be in the foreground."))
	bool bOutgoingVisibilityUseWorldSpace = true;

	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Show Incoming Connections"))
	bool bShowIncomingConnections = true;
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Connection Color"))
	FColor IncomingConnectionColor = FColor(0, 200, 255);
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Line Thickness", ClampMin = "0.1", ClampMax = "10.0"))
	float IncomingConnectionThickness = 1.0f;
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Connection Dashed lines", ToolTip = "Dashed Distance is defined by line thickness parameter"))
	bool bIncomingConnectionDashedLines = false;
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Connections Use World Space", ToolTip = "If true, the connection lines will be in world space, otherwise they will be in the foreground."))
	bool bIncomingVisibilityUseWorldSpace = true;

	// Connectors are automatically added to targets when they are set as an output of a connector.
	// This will only remove the instanced versions if there isn't a permanent one already on the actor.
	UPROPERTY(Config, EditAnywhere, Category = "Cleanup", meta = (DisplayName = "Auto-Remove Unused Connectors",
	    ToolTip = "If true, automatically removes connector components that have no incoming or outgoing connections and were dynamically added, when selecting the actor"))
	bool bAutoRemoveUnusedConnectors = false;
};
