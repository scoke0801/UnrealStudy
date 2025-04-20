#include "PGMontageAbility.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UPGMontageAbility::UPGMontageAbility()
{
    // 기본값 설정
    bEndAbilityWithMontage = true;
    MontageSection = NAME_None;
}

void UPGMontageAbility::K2_OnActivate_Implementation()
{
    Super::K2_OnActivate_Implementation();
    
    if (!AbilityMontage || !OwningActor)
    {
        // 몽타주가 없거나 소유자가 없으면 능력 종료
        End(true);
        return;
    }
    
    // 소유자가 캐릭터인지 확인
    ACharacter* OwningCharacter = Cast<ACharacter>(OwningActor);
    if (!OwningCharacter)
    {
        End(true);
        return;
    }
    
    // 애니메이션 인스턴스 획득
    AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        End(true);
        return;
    }
    
    // 몽타주 재생
    float MontageLength = 0.0f;
    if (MontageSection != NAME_None)
    {
        MontageLength = AnimInstance->Montage_Play(AbilityMontage, 1.0f);
        if (MontageLength > 0.0f)
        {
            AnimInstance->Montage_JumpToSection(MontageSection, AbilityMontage);
        }
    }
    else
    {
        MontageLength = AnimInstance->Montage_Play(AbilityMontage, 1.0f);
    }
    
    if (MontageLength <= 0.0f)
    {
        // 몽타주 재생 실패
        End(true);
        return;
    }
    
    // 현재 몽타주 저장
    CurrentMontageInstance = AbilityMontage;
    
    // 몽타주 종료 콜백 등록
    if (bEndAbilityWithMontage)
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &UPGMontageAbility::OnMontageEnded);
    }
    
    // 노티파이 핸들러 등록
    AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UPGMontageAbility::HandleNotifyBegin);
    AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UPGMontageAbility::HandleNotifyEnd);
}

void UPGMontageAbility::K2_OnEnd_Implementation(bool bWasCancelled)
{
    Super::K2_OnEnd_Implementation(bWasCancelled);
    
    // 몽타주 중지
    if (AnimInstance && CurrentMontageInstance)
    {
        // 몽타주가 재생 중인지 확인
        if (AnimInstance->Montage_IsPlaying(CurrentMontageInstance))
        {
            // 즉시 중지할지 블렌드 아웃할지 결정
            if (bWasCancelled)
            {
                AnimInstance->Montage_Stop(0.1f, CurrentMontageInstance);
            }
            else
            {
                AnimInstance->Montage_Stop(0.25f, CurrentMontageInstance);
            }
        }
        
        // 콜백 제거
        AnimInstance->OnMontageEnded.RemoveDynamic(this, &UPGMontageAbility::OnMontageEnded);
        AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UPGMontageAbility::HandleNotifyBegin);
        AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &UPGMontageAbility::HandleNotifyEnd);
    }
    
    // 변수 초기화
    AnimInstance = nullptr;
    CurrentMontageInstance = nullptr;
    MontageEndedDelegate.Reset();
}

void UPGMontageAbility::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == CurrentMontageInstance && IsActive())
    {
        End(bInterrupted);
    }
}

void UPGMontageAbility::HandleNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    // 현재 몽타주가 우리 몽타주인지 확인
    if (BranchingPointPayload.MontageInstanceID != INDEX_NONE && 
        AnimInstance && 
        AnimInstance->GetActiveMontageInstance() && 
        AnimInstance->GetActiveMontageInstance()->Montage == CurrentMontageInstance)
    {
        OnAbilityNotifyBegin(NotifyName);
    }
}

void UPGMontageAbility::HandleNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    // 현재 몽타주가 우리 몽타주인지 확인
    if (BranchingPointPayload.MontageInstanceID != INDEX_NONE && 
        AnimInstance && 
        AnimInstance->GetActiveMontageInstance() && 
        AnimInstance->GetActiveMontageInstance()->Montage == CurrentMontageInstance)
    {
        OnAbilityNotifyEnd(NotifyName);
    }
}
