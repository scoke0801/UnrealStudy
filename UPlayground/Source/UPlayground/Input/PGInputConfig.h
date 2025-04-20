#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGInputConfig.generated.h"

/**
 * 향상된 입력 시스템을 위한 입력 설정
 */
UCLASS()
class UPLAYGROUND_API UPGInputConfig : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPGInputConfig();
    
    // 기본 움직임 입력
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    class UInputAction* MoveAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    class UInputAction* LookAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    class UInputAction* JumpAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    class UInputAction* SprintAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Movement")
    class UInputAction* DodgeAction;
    
    // 전투 관련 입력
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* PrimaryAttackAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* SecondaryAttackAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* TargetAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Combat")
    class UInputAction* ClearTargetAction;
    
    // 능력 관련 입력
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Abilities")
    class UInputAction* Ability1Action;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Abilities")
    class UInputAction* Ability2Action;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Abilities")
    class UInputAction* Ability3Action;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Abilities")
    class UInputAction* Ability4Action;
    
    // 상호작용 관련 입력
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Interaction")
    class UInputAction* InteractAction;
    
    // 메뉴 관련 입력
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|UI")
    class UInputAction* InventoryAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|UI")
    class UInputAction* QuestLogAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|UI")
    class UInputAction* CharacterAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|UI")
    class UInputAction* PauseAction;
};
