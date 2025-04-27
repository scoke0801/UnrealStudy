// Copyright (c) 2025. All Rights Reserved.

#include "PGRPGManagersFeature.h"

#include "UPlayground/Managers/PGInventoryManager.h"
#include "UPlayground/Managers/PGQuestManager.h"
#include "UPlayground/Managers/PGSaveManager.h"

UPGRPGManagersFeature::UPGRPGManagersFeature()
{
	// 기본 RPG 매니저 클래스 설정
	ManagerClasses.Add(UPGInventoryManager::StaticClass());
	ManagerClasses.Add(UPGQuestManager::StaticClass());
	ManagerClasses.Add(UPGSaveManager::StaticClass());
	
	// 필요한 다른 RPG 매니저 클래스 추가 가능
	// 예: 캐릭터 매니저, 대화 매니저, 전투 매니저 등
}
