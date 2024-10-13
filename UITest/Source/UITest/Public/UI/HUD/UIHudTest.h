// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/UIHudBase.h"
#include "UIHudTest.generated.h"

/**
 * 
 */
UCLASS()
class UITEST_API UUIHudTest : public UUIHudBase
{
	GENERATED_BODY()
	
protected: 
	virtual void NativeConstruct() override;
};
