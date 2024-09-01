// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTest/Character/CWCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/CWCharacterAIInterface.h"
#include "CWNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config = CombatWarehouse)
class ACWNonPlayer : public ACWCharacterBase, public ICWCharacterAIInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(config)
	TArray<FSoftObjectPath> _npcMeshes;

	TSharedPtr<FStreamableHandle> _npcMeshHandle;

	FAICharacterAttackFinished OnAttackFinished;

public:
	ACWNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted();

	virtual void NotifyComboActionEnd() override;
	// Interface
public:
	virtual float GetAIPatrolRadius()override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed()   override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	virtual void AttackByAI() override;
};
