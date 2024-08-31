// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWNonPlayer.h"

#include "Engine/AssetManager.h"

ACWNonPlayer::ACWNonPlayer()
{
}

void ACWNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (_npcMeshes.Num() == 0)
	{
		return;
	}

	int32 RandIndex = FMath::RandRange(0, _npcMeshes.Num() - 1);
	_npcMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		_npcMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &ACWNonPlayer::NPCMeshLoadCompleted));
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

void ACWNonPlayer::NPCMeshLoadCompleted()
{
	if (_npcMeshHandle.IsValid())
	{
		if (USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(_npcMeshHandle->GetLoadedAsset()))
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	_npcMeshHandle->ReleaseHandle();
}
