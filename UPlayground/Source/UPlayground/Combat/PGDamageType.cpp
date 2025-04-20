#include "PGDamageType.h"

UPGDamageType::UPGDamageType()
{
    // 기본값 설정
    DamageType = EPGDamageType::Physical;
    
    // 기본 데미지 속성
    bCanCritical = true;
    bIgnoresArmor = false;
    bCausesStatusEffect = false;
    
    // 기본 상태 효과 설정
    StatusEffectDuration = 5.0f;
    StatusEffectChance = 0.25f;
}
