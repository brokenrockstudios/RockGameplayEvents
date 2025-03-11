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
	FColor OutgoingConnectionColor = FColor(0, 255, 0);
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Line Thickness", ClampMin = "0.1", ClampMax = "10.0"))
	float OutgoingConnectionThickness = 1.0f;
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Connection Dashed Lines", ToolTip = "Dashed Distance is defined by line thickness parameter"))
	bool bOutgoingConnectionDashedLines = false;
	UPROPERTY(Config, EditAnywhere, Category = "OutgoingConnections", meta = (DisplayName = "Outgoing Connections Use World Space", ToolTip = "If true, the connection lines will be in world space, otherwise they will be in the foreground."))
	bool bOutgoingVisibilityUseWorldSpace = true;

	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Show Incoming Connections"))
	bool bShowIncomingConnections = true;
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Connection Color"))
	FColor IncomingConnectionColor = FColor(255, 255, 255);
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Line Thickness", ClampMin = "0.1", ClampMax = "10.0"))
	float IncomingConnectionThickness = 1.0f;
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Connection Dashed lines", ToolTip = "Dashed Distance is defined by line thickness parameter"))
	bool bIncomingConnectionDashedLines = false;
	UPROPERTY(Config, EditAnywhere, Category = "IncomingConnections", meta = (DisplayName = "Incoming Connections Use World Space", ToolTip = "If true, the connection lines will be in world space, otherwise they will be in the foreground."))
	bool bIncomingVisibilityUseWorldSpace = true;
};
