// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CWCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "BasicTest/Data/CWComboAttackData.h"

// Sets default values
ACWCharacterBase::ACWCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ExternalAssets/BossyEnemy/SkeletalMesh/SK_Mannequin_UE4_WithWeapon.SK_Mannequin_UE4_WithWeapon"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/Blueprint/ABP_WeaponCharacter.ABP_WeaponCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UCWCharacterControlData> ShoulderDataRef(TEXT("/Game/CharacterControl/DA_CharacterControlQuater.DA_CharacterControlQuater"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCWCharacterControlData> QuaterDataRef(TEXT("/Game/CharacterControl/DA_CharacterControlShoulder.DA_CharacterControlShoulder"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}
}
 
void ACWCharacterBase::SetCharacterControlData(const UCWCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void ACWCharacterBase::ProcessComboCommand(float InSpeed)
{
	if (_currentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!_comboTimerHandle.IsValid())
	{
		_hasNextComboCommand = false;
	}
	else
	{
		_hasNextComboCommand = true;
	}
}

void ACWCharacterBase::ComboActionBegin()
{
	_currentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(_comboActionMontage, AttackSpeedRate);
		
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ACWCharacterBase::ComboActionEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, _comboActionMontage);
	}

	_comboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ACWCharacterBase::ComboActionEnd(UAnimMontage* InTargetMontage, bool InIsEnded)
{
	ensure(_currentCombo != 0);

	_currentCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ACWCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = _currentCombo - 1;
	ensure(_comboActionData->_effectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (_comboActionData->_effectiveFrameCount[ComboIndex] / _comboActionData->_frameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(_comboTimerHandle, this, &ACWCharacterBase::CheckCombo, ComboEffectiveTime, false);
	}
}

void ACWCharacterBase::CheckCombo()
{
	_comboTimerHandle.Invalidate();
	if (_hasNextComboCommand)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			_currentCombo = FMath::Clamp(_currentCombo + 1, 1, _comboActionData->_maxComboCount);
			FName NextSection = *FString::Printf(TEXT("%s%d"), *(_comboActionData->_montageSectionNamePrefix), _currentCombo);
			AnimInstance->Montage_JumpToSection(NextSection, _comboActionMontage);
			
			SetComboCheckTimer();

			_hasNextComboCommand = false;
		}
	}
}
