// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTest/Character/CWCharacterBase.h"
#include "CWNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ACWNonPlayer : public ACWCharacterBase
{
	GENERATED_BODY()
	
public:
	ACWNonPlayer();

protected:
	void SetDead() override;
};
