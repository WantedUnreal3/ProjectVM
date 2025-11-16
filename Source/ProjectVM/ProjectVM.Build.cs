// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class ProjectVM : ModuleRules
{
	public ProjectVM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// 폴더
		PublicIncludePaths.AddRange(new[] { "ProjectVM" });

		// 기존 모듈 목록
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		// 모듈 추가 (작업자: 나희영)
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem", "Niagara"});

		// 모듈 추가 (작업자: 이상윤)
        PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
        
        PublicDefinitions.Add("HERO_SKILL_DEBUG=0");
    }
}
