// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Swd : ModuleRules
{
	public Swd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "AIModule", "InputCore","GameplayTasks", "HeadMountedDisplay", "UMG", "Slate", "SlateCore"
		});
	}
}