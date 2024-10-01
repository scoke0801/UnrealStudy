// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UIWidgetComponentTest.generated.h"

/**
 * 
 */
UCLASS()
class COMBATWAREHOUSE_API UUIWidgetComponentTest : public UWidgetComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUIUpdate, float /**/);
	
	DECLARE_DELEGATE_RetVal(bool, FOnCheckIsCanEnable);
	DECLARE_DELEGATE_RetVal(bool, FOnCheckIsCanDisable);

public:
	FOnUIUpdate OnUIUpdate;

	TArray<FOnCheckIsCanEnable> EnableCheckList;
	TArray<FOnCheckIsCanDisable> DisableCheckList;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void EnableTick();
	void DisableTick();
};

