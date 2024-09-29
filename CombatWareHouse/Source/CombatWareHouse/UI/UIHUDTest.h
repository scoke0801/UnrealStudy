// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHUDTest.generated.h"

class UButton;

DECLARE_LOG_CATEGORY_EXTERN(LogJH, Log, All);
/**
 * 
 */
UCLASS()
class COMBATWAREHOUSE_API UUIHUDTest : public UUserWidget
{
	GENERATED_BODY()

private:
	bool _isRunning = false;
	double _prevProcessTime =0;
protected:
	UPROPERTY(EditAnywhere, Category = UI,Meta = (BindWidget))
	UButton* _startButton;


	UPROPERTY(EditAnywhere, Category = UI,Meta = (BindWidget))
	UButton* _endButton;

public:
	virtual void NativeOnInitialized() override;

private:
	void Process();

	UFUNCTION()
	void OnClickedStartButton();

	UFUNCTION()
	void OnClicedEndButtion();
};
