// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PluginTest_5_4_4 : ModuleRules
{
	public PluginTest_5_4_4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Sentry" });
	}
}
