// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

using UnrealBuildTool;

public class RockGameplayEvents : ModuleRules
{
	public RockGameplayEvents(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "GameplayTags",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
				"UnrealEd",
				"BlueprintGraph"
			}
			);
	}
}
