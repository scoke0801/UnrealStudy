#include "PGAttackData.h"

UPGAttackData::UPGAttackData()
{
    // 기본값 설정
    AttackID = NAME_None;
    
    // 기본 트레이스 채널
    TraceChannel = ECC_Pawn;
    
    // 기본 공격 형태
    AttackShape = EPGAttackShape::Melee;
    
    // 기본 공격 속성
    BaseDamage = 10.0f;
    Range = 150.0f;
    Radius = 50.0f;
    Angle = 45.0f;
    
    // 기본 발사체 설정
    ProjectileSpeed = 1000.0f;
    
    // 기본 게임플레이 속성
    Cooldown = 1.0f;
    bRequiresTarget = false;
    bCanBeBlocked = true;
    bCanBeEvaded = true;
}
