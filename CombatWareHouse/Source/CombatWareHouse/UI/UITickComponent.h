// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UITickComponent.generated.h"

/**
 * 
 */
UCLASS()
class COMBATWAREHOUSE_API UUITickComponent : public UUserWidget
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
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void EnableTick();
	void DisableTick();
};
