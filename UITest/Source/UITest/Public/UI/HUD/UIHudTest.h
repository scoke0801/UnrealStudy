// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/UIHudBase.h"
#include "UIHudTest.generated.h"

class UEditableTextBox;

/**
 * 
 */
UCLASS()
class UITEST_API UUIHudTest : public UUIHudBase
{
	GENERATED_BODY()
	
	struct CachedLocationInfo
	{
		TWeakObjectPtr<class UUserWidget> _widget;
		FVector2D _location;
	};
protected:
	UPROPERTY(EditAnywhere, Category = "UI", Meta = (BindWidget))
	UEditableTextBox* _editableTextBox1;

	UPROPERTY(EditAnywhere, Category = "UI", Meta = (BindWidget))
	UEditableTextBox* _editableTextBox2;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> _valueTextClass;

	UPROPERTY(Transient)
	TObjectPtr<AActor> _targetActor = nullptr;

	TMap<int32, CachedLocationInfo> _locationMap;

private:
	bool _isStarted = false;

	double _elapsedTime = 0.0f;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnTextCommittedText_1(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void OnTextCommittedText_2(const FText& Text, ETextCommit::Type CommitMethod);

private:
	void FindActorByBame(FName InTargetName);

	FVector GetLocationOfActorSocket(FName InSocketName);

	void AddTextValueWidget(int32 InValue);

	void OnRemoveNotify(int32 InKeyValue);

	FVector2D GenerateRandLocation(const FVector2D InBaseLocation) const;
};
