// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

using UnrealBuildTool;

public class RockGameplayEventsEditor : ModuleRules
{
	public RockGameplayEventsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"RockGameplayEvents",
				"UnrealEd"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
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
				"DeveloperSettings",
			}
		);
	}
}