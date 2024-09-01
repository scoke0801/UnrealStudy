// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWUIHUDIngame.h"
#include "BasicTest/Character/Interface/CWCharacterHUDInterface.h"

#include "CWUIHpBar.h"
#include "CWUICharacterStat.h"

UCWUIHUDIngame::UCWUIHUDIngame(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCWUIHUDIngame::UpdateStat(const FCWCharacterStat& InBaseStat, const FCWCharacterStat& InModifierStat)
{
	FCWCharacterStat TotalSTat = InBaseStat + InModifierStat;

	_hpBar->UpdateStat(InBaseStat, InModifierStat);
	
	_characterStat->UpdateStat(InBaseStat, InModifierStat);
}

void UCWUIHUDIngame::UpdateHpBar(float InNewHp)
{
	_hpBar->UpdateHpBar(InNewHp);
}

void UCWUIHUDIngame::NativeConstruct()
{
	Super::NativeConstruct();

	if (ICWCharacterHUDInterface* HUDPawn = Cast< ICWCharacterHUDInterface>(GetOwningPlayerPawn()))
	{
		HUDPawn->SetupHUDWIdget(this);
	}

}
