// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWNonPlayer.h"
#include "BasicTest/AI/CWAIController.h"
#include "CWCharacterStatComponent.h"

#include "Engine/AssetManager.h"


ACWNonPlayer::ACWNonPlayer()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		// 로딩 된 이후에, 보이도록 할 것임
		MeshComp->SetHiddenInGame(true);
	}

	AIControllerClass = ACWAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Blueprint/Animation/ABP_Enemy.ABP_Enemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
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

void ACWNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	OnAttackFinished.ExecuteIfBound();
}

float ACWNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float ACWNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float ACWNonPlayer::GetAIAttackRange()
{
	return _statComp->GetTotalStat().AttackRange + _statComp->GetAttackRadius() * 2;
}

float ACWNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void ACWNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ACWNonPlayer::AttackByAI()
{
	ProcessComboCommand(1.0f);
}
