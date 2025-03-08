// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

using UnrealBuildTool;


// Note
// Visualizers rely on GUnrealEd which isn't available unless the module is PostEngineInit

public class RockGameplayEventsVisualizers : ModuleRules
{
	public RockGameplayEventsVisualizers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"RockGameplayEvents",
				"UnrealEd"
			]
		);
		
		PrivateDependencyModuleNames.AddRange(
			[
				"BlueprintGraph",
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"LevelEditor",
				"LiveCoding",
				"PlacementMode",
				"PropertyEditor",
				"Slate",
				"SlateCore",
				"ToolWidgets",
				"UnrealEd",
			]
		);
	}
}
