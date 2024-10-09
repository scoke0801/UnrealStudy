// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "UIHUDTest.generated.h"

class UButton;
class UEditableText;
class UEditableTextBox;

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

	UPROPERTY(EditAnywhere, Category = UI, Meta = (BindWidget))
	UEditableText* _editableText;

	UPROPERTY(EditAnywhere, Category = UI, Meta = (BindWidget))
	UEditableTextBox* _editableTextBox;
public:
	virtual void NativeOnInitialized() override;

private:
	void Process();

	UFUNCTION()
	void OnClickedStartButton();

	UFUNCTION()
	void OnClicedEndButtion();

	UFUNCTION()
	void OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnTextCommitedBox(const FText& Text, ETextCommit::Type CommitMethod);
};
