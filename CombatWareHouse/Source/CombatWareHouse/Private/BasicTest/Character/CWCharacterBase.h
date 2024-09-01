// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicTest/Animation/Interface/CWAnimationAttackInterface.h"
#include "BasicTest/UI/Interface/CWCHaracterWidgetInterface.h"
#include "Interface/CWCharacterItemIntercae.h"
#include "CWCharacterBase.generated.h"

class UCWCharacterControlData;
class UAnimMontage;
class UCWComboAttackData;
class UCWWidgetComponentBase;
class UCWCharacterStatComponent;
class UCWItemData;

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UCWItemData* /*InItemData*/);
DECLARE_LOG_CATEGORY_EXTERN(LogCWCharacter, Log, All);

USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	
	explicit FTakeItemDelegateWrapper() {}
	explicit FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : _onTakeItemDelegate(InItemDelegate) {}
	
	FOnTakeItemDelegate _onTakeItemDelegate;
};

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
	class ACWCharacterBase : public ACharacter,
		public ICWAnimationAttackInterface, public ICWCharacterWidgetInterface, public ICWCharacterItemIntercae
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, UCWCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage>  _comboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCWComboAttackData> _comboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> _deadMontage;
	
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper	> _takeItemActions;

	//Weapon
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> _weapon;

	// stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCWCharacterStatComponent> _statComp;

	// UI
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCWWidgetComponentBase> _uiHpBar;

protected:
	float _deadEventDelayTime = 5.0f;

private:
	int32 _currentCombo = 0;
	FTimerHandle _comboTimerHandle;
	bool _hasNextComboCommand = false;

public:
	// Sets default values for this character's properties
	ACWCharacterBase();

	virtual void PostInitializeComponents() override;

public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

protected:
	virtual void SetCharacterControlData(const UCWCharacterControlData* InCharacterControlData);

	// Montage
protected:
	void ProcessComboCommand(float InSpeed);

	void ComboActionBegin();
	void ComboActionEnd(UAnimMontage* InTargetMontage, bool InIsEnded);

	virtual void NotifyComboActionEnd();

	// Interface
protected:
	void AttackHitCheck() override;

	void SetupCharacterWidget(UCWUserWIdgetBase* InUserWidget) override;

	void TakeItem(UCWItemData* InItemData) override;
protected:
	virtual float TakeDamage(float InDamageAmount, FDamageEvent const& InDamageEvent, AController* InEventInstigator, AActor* InDamageCauser) override;

	virtual void SetDead();

private:
	void SetComboCheckTimer();
	void CheckCombo();

	void PlayDeadAnimation();

private:
	virtual void DrinkPotion(UCWItemData* InItemData); 
	virtual void EquipWeapon(UCWItemData* InItemData);
	virtual void ReadScroll(UCWItemData* InItemData);

};
