#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGAbilityTypes.h"
#include "PGSkillData.generated.h"

// 레벨별 수치 배열을 담을 구조체
USTRUCT(BlueprintType)
struct FPGLevelValueArray
{
    GENERATED_BODY()
        
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<float> Values;
};
    
/**
 * 스킬 설정을 위한 데이터 에셋
 */
UCLASS()
class UPLAYGROUND_API UPGSkillData : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPGSkillData();
    
    // 기본 스킬 정보
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    FName SkillID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    FText SkillName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    FText SkillDescription;
    
    // 스킬 레벨 정보
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Level")
    int32 MaxLevel;
    
    // 스킬 타입 및 분류
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Type")
    EPGAbilityTargetType TargetType;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Type")
    TArray<EPGAbilityEffectType> EffectTypes;
    
    // 스킬 비용
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs")
    TArray<FPGResourceCost> ResourceCosts;
    
    // 시각 효과
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Visuals")
    class UTexture2D* SkillIcon;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Visuals")
    class UParticleSystem* SkillEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Visuals")
    class UNiagaraSystem* SkillNiagaraEffect;
    
    // 오디오
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Audio")
    class USoundBase* SkillSound;
    
    // 게임플레이 매개변수
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Gameplay")
    float Cooldown;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Gameplay")
    float Range;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Gameplay")
    float Radius;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Gameplay")
    float CastTime;
    
    // 레벨별 수치 테이블
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Scaling")
    TMap<FName, FPGLevelValueArray> LevelValues;
    
    // 특정 레벨의 값 가져오기
    UFUNCTION(BlueprintCallable, Category = "Skill")
    float GetValueAtLevel(FName ValueName, int32 Level) const;
};
