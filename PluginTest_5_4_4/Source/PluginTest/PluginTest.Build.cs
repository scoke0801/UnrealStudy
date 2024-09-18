// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PluginTest : ModuleRules
{
	public PluginTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Sentry" });
	}
}