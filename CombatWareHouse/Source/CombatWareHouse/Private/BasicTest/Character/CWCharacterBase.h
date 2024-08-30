// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicTest/Animation/CWAnimationAttackInterface.h"
#include "CWCharacterBase.generated.h"

class UCWCharacterControlData;
class UAnimMontage;
class UCWComboAttackData;
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
	class ACWCharacterBase : public ACharacter, public ICWAnimationAttackInterface
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

protected:
	float _deadEventDelayTime = 5.0f;

private:
	int32 _currentCombo = 0;
	FTimerHandle _comboTimerHandle;
	bool _hasNextComboCommand = false;

public:
	// Sets default values for this character's properties
	ACWCharacterBase();


protected:
	virtual void SetCharacterControlData(const UCWCharacterControlData* InCharacterControlData);

	// Montage
protected:
	void ProcessComboCommand(float InSpeed);

	void ComboActionBegin();
	void ComboActionEnd(UAnimMontage* InTargetMontage, bool InIsEnded);

	// Interface
protected:
	void AttackHitCheck() override;
	
	virtual float TakeDamage(float InDamageAmount, FDamageEvent const& InDamageEvent, AController* InEventInstigator, AActor* InDamageCauser) override;

	virtual void SetDead();

private:
	void SetComboCheckTimer();
	void CheckCombo();

	void PlayDeadAnimation();

};
