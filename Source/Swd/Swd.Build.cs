// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Swd : ModuleRules
{
	public Swd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore", "NavigationSystem", "ApexDestruction", "AIModule",
			"GameplayTasks", "Niagara"
		});
	}
}