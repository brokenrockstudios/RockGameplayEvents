// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

using UnrealBuildTool;

public class RockGameplayEvents : ModuleRules
{
	public RockGameplayEvents(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		// PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
		
		PublicDependencyModuleNames.AddRange(
			[
				"Core", "GameplayTags"
			]
		);

		PrivateDependencyModuleNames.AddRange(
			[
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Niagara"
			]
		);
	}
}