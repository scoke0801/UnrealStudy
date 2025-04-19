// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGBaseCharacter.h"
#include "PGNPC.generated.h"

// NPC 유형 열거형
UENUM(BlueprintType)
enum class EPGNPCType : uint8
{
    Neutral     UMETA(DisplayName = "Neutral"),
    Friendly    UMETA(DisplayName = "Friendly"),
    Hostile     UMETA(DisplayName = "Hostile"),
    Merchant    UMETA(DisplayName = "Merchant"),
    QuestGiver  UMETA(DisplayName = "Quest Giver")
};

UCLASS()
class UPLAYGROUND_API APGNPC : public APGBaseCharacter
{
    GENERATED_BODY()

public:
    APGNPC();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // NPC 상호작용 메서드 오버라이드
    virtual void Interact(APGBaseCharacter* Interactor) override;
    virtual bool CanBeInteractedWith() const override;

protected:
    // NPC 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
    EPGNPCType NPCType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
    FString NPCName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
    FString DialogueIntroduction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
    TArray<class UObject*> AvailableQuests; // 일단 UObject로 변경, 퀘스트 클래스가 정의되면 수정 필요

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
    bool bCanTrade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Properties")
    TArray<class UPGItem*> MerchantInventory;

    // NPC 행동 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Behavior")
    float InteractionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Behavior")
    bool bStationary;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Behavior")
    bool bIsImportant; // 중요 NPC 여부 (퀘스트 관련, 스토리 관련 등)

    // NPC 대화 시스템
    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void StartDialogue(class APGPlayerCharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void EndDialogue();

    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void OfferQuests(class APGPlayerCharacter* Player);
    
    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void OpenShop(class APGPlayerCharacter* Player);

    // 이벤트 델리게이트
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNPCInteracted, APGNPC*, NPC, class APGPlayerCharacter*, Player);
    
    UPROPERTY(BlueprintAssignable, Category = "NPC Events")
    FOnNPCInteracted OnNPCInteracted;
};