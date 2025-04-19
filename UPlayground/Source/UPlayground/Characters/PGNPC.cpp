// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGNPC.h"
#include "PGPlayerCharacter.h"
#include "../Items/PGItem.h"
#include "../Managers/PGQuestTypes.h"

APGNPC::APGNPC()
{
    // 기본값 설정
    NPCType = EPGNPCType::Neutral;
    NPCName = "Generic NPC";
    DialogueIntroduction = "Hello, traveler!";
    bCanTrade = false;
    InteractionRadius = 200.0f;
    bStationary = true;
    bIsImportant = false;
}

void APGNPC::BeginPlay()
{
    Super::BeginPlay();
}

void APGNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // NPC 행동 로직은 여기서 처리할 수 있음
}

void APGNPC::Interact(APGBaseCharacter* Interactor)
{
    Super::Interact(Interactor);
    
    // 플레이어 캐릭터인지 확인
    APGPlayerCharacter* PlayerCharacter = Cast<APGPlayerCharacter>(Interactor);
    if (PlayerCharacter)
    {
        // NPC와 플레이어 상호작용 로직
        StartDialogue(PlayerCharacter);
        
        // 이벤트 발생
        OnNPCInteracted.Broadcast(this, PlayerCharacter);
    }
}

bool APGNPC::CanBeInteractedWith() const
{
    // NPC가 상호작용 가능한지 여부 결정
    return Super::CanBeInteractedWith();
}

void APGNPC::StartDialogue(APGPlayerCharacter* Player)
{
    // 대화 시작 로직
    // 대화 시스템 구현은 제외
}

void APGNPC::EndDialogue()
{
    // 대화 종료 로직
}

void APGNPC::OfferQuests(APGPlayerCharacter* Player)
{
    // 퀘스트 제공 로직
}

void APGNPC::OpenShop(APGPlayerCharacter* Player)
{
    // 상점 UI 열기 로직
}