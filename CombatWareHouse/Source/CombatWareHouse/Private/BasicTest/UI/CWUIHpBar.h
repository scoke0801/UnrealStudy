// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CWUserWIdgetBase.h"
#include "CWUIHpBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class UCWUIHpBar : public UCWUserWIdgetBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = Ui, meta = (BindWidgetOptinal))
	TObjectPtr<UProgressBar> _hpProgressBar;

	UPROPERTY()
	float _maxHp;

public:
	UCWUIHpBar(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float InMaxHp) { _maxHp = InMaxHp; }
	void UpdateHpBar(float InHp);
};
