// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWNonPlayer.h"

ACWNonPlayer::ACWNonPlayer()
{
}

void ACWNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), _deadEventDelayTime, false);

}
