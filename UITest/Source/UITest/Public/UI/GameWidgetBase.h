// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/UICommon.h"
#include "GameWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class UITEST_API UGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "CustomUI")
	EUIType _uiType = EUIType::None;

public:
	FORCEINLINE EUIType GetUIType() const { return _uiType; }


};
