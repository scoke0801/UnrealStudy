#include "PGSkillData.h"

UPGSkillData::UPGSkillData()
{
    // 기본값 설정
    SkillID = NAME_None;
    MaxLevel = 5;
    TargetType = EPGAbilityTargetType::Self;
    
    // 기본 게임플레이 값
    Cooldown = 10.0f;
    Range = 0.0f;
    Radius = 0.0f;
    CastTime = 0.0f;
}

float UPGSkillData::GetValueAtLevel(FName ValueName, int32 Level) const
{
    // 레벨 유효성 검사
    const int32 ValidLevel = FMath::Clamp(Level, 1, MaxLevel);
    
    // 값이 존재하는지 확인
    if (LevelValues.Contains(ValueName))
    {
        const FPGLevelValueArray& LevelValueArray = LevelValues[ValueName];
        
        // 레벨에 맞는 값이 존재하는지 확인
        if (LevelValueArray.Values.IsValidIndex(ValidLevel - 1))
        {
            return LevelValueArray.Values[ValidLevel - 1];
        }
    }
    
    // 값이 없으면 0 반환
    return 0.0f;
}
