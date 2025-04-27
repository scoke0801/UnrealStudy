// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UPlayground : ModuleRules
{
	public UPlayground(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // C++ 20 사용 설정
        CppStandard = CppStandardVersion.Cpp20;
		
		// 모듈 헤더 공개 설정
		PublicIncludePaths.AddRange(new string[] {
			"UPlayground",
			"UPlayground/Equipment"
		});
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayTags",
            "DeveloperSettings",
			"UMG",
			"Niagara",
			"CommonUI" // CommonUI 모듈 추가
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
            "GameplayTasks", 
            "GameFeatures",
            "ModularGameplay"
		});

		// UI 시스템을 위한 Slate 및 SlateCore 의존성 추가
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
