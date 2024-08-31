// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTest/Character/CWCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "CWNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config = CombatWarehouse)
class ACWNonPlayer : public ACWCharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(config)
	TArray<FSoftObjectPath> _npcMeshes;

	TSharedPtr<FStreamableHandle> _npcMeshHandle;

public:
	ACWNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted();

};
