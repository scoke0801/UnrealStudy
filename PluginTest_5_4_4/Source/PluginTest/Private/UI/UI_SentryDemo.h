// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SentryDemo.generated.h"

class UButton;
class UComboBoxString;

/**
 * 
 */
UCLASS(Meta = (DisableNativeTick))
class UUI_SentryDemo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_CaptureMessage;

	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_CaptureMessageWithScope;

	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_CaptureEvent;
	
	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_CaptureEventWithScope;

	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_Performance;
	
	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_Performance2;

	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UButton* _Button_Crash;

	UPROPERTY(BlueprintReadWrite, Category = UI, Meta = (BindWidget))
	UComboBoxString* _ComboBoxString;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void OnClickedCaptureMessage();
	
	UFUNCTION()
	void OnClickedCaptureMessageWithScope();

	UFUNCTION()
	void OnClickedScopeConfigure();

	UFUNCTION()
	void OnClickedCaptureEvent();

	UFUNCTION()
	void OnClickedCaptureEventWithScope();
	
	UFUNCTION()
	void OnCLickedTerminate();
	
	UFUNCTION()
	void OnClickedPerformance();
	
	UFUNCTION()
	void OnClickedPerformance2();
};
