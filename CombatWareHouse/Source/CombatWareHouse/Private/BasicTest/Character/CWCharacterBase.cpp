// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWCharacterBase.h"
#include "BasicTest/Data/CWComboAttackData.h"
#include "BasicTest/Physics/CWCollision.h"
#include "BasicTest/UI/CWUserWIdgetBase.h"
#include "BasicTest/UI/CWWidgetComponentBase.h"
#include "BasicTest/UI/CWUIHpBar.h"
#include "BasicTest/Data/CWWeaponItemData.h"

#include "CWCharacterControlData.h"
#include "CWCharacterStatComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Animation/AnimMontage.h"

#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY(LogCWCharacter);

// Sets default values
ACWCharacterBase::ACWCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 92.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_CWCAPSULE);

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
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin.SKM_UEFN_Mannequin"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Blueprint/Animation/ABP_BasicCharacter.ABP_BasicCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UCWCharacterControlData> ShoulderDataRef(TEXT("/Game/Data/CharacterControl/DA_CharacterControlQuater.DA_CharacterControlQuater"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCWCharacterControlData> QuaterDataRef(TEXT("/Game/Data/CharacterControl/DA_CharacterControlShoulder.DA_CharacterControlShoulder"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack"));
	if (ComboActionMontageRef.Object)
	{
		_comboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCWComboAttackData> ComboActionDataRef(TEXT("/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack"));
	if (ComboActionDataRef.Object)
	{
		_comboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/Animation/Montage/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		_deadMontage = DeadMontageRef.Object;
	}

	_weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	_weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	_statComp = CreateDefaultSubobject<UCWCharacterStatComponent>(TEXT("Stat"));

	if (_uiHpBar = CreateDefaultSubobject<UCWWidgetComponentBase>(TEXT("Widget")))
	{
		_uiHpBar->SetupAttachment(GetMesh());
		_uiHpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

		static ConstructorHelpers::FClassFinder<UCWUserWIdgetBase> HpBarWidgetRef(TEXT("/Game/Blueprint/UI/WBP_HpBar.WBP_HpBar_C"));
		if (HpBarWidgetRef.Class)
		{
			_uiHpBar->SetWidgetClass(HpBarWidgetRef.Class);
			_uiHpBar->SetWidgetSpace(EWidgetSpace::Screen);
			_uiHpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
			_uiHpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	_takeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACWCharacterBase::EquipWeapon))); 
	_takeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACWCharacterBase::DrinkPotion)));
	_takeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACWCharacterBase::ReadScroll)));
}

void ACWCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_statComp->_onHpZeroDelegate.AddUObject(this, &ACWCharacterBase::SetDead);
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

void ACWCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 100.0f;

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_CWACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}

void ACWCharacterBase::SetupCharacterWidget(UCWUserWIdgetBase* InUserWidget)
{
	if (UCWUIHpBar* HpBarWideget = Cast<UCWUIHpBar>(InUserWidget))
	{
		HpBarWideget->SetMaxHp(_statComp->GetMaxHp());
		HpBarWideget->UpdateHpBar(_statComp->GetCurrentHp());

		_statComp->_onHpChangedDelegate.AddUObject(HpBarWideget, &UCWUIHpBar::UpdateHpBar);
	}
}

void ACWCharacterBase::TakeItem(UCWItemData* InItemData)
{
	if (nullptr == InItemData)
	{
		return;
	}
	
	uint8 Index = StaticCast<uint8>(InItemData->_type);
	if (false == _takeItemActions.IsValidIndex(Index))
	{
		return;
	}
	
	_takeItemActions[Index]._onTakeItemDelegate.ExecuteIfBound(InItemData);
}

float ACWCharacterBase::TakeDamage(float InDamageAmount, FDamageEvent const& InDamageEvent, AController* InEventInstigator, AActor* InDamageCauser)
{
	Super::TakeDamage(InDamageAmount, InDamageEvent, InEventInstigator, InDamageCauser);

	_statComp->ApplyDamage(InDamageAmount);

	return InDamageAmount;
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

void ACWCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();

	SetActorEnableCollision(false);
	_uiHpBar->SetHiddenInGame(true);
}

void ACWCharacterBase::PlayDeadAnimation()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(_deadMontage);
	}
}

void ACWCharacterBase::DrinkPotion(UCWItemData* InItemData)
{
	UE_LOG(LogCWCharacter, Log, TEXT("DrinkPotion"));
}

void ACWCharacterBase::EquipWeapon(UCWItemData* InItemData)
{
	UE_LOG(LogCWCharacter, Log, TEXT("EquipWeapon"));

	if (nullptr == InItemData)
	{
		return;
	}

	if (UCWWeaponItemData* WeaponItemData = Cast<UCWWeaponItemData>(InItemData))
	{
		if (WeaponItemData->_weaponMesh.IsPending())
		{
			WeaponItemData->_weaponMesh.LoadSynchronous();
		}

		_weapon->SetSkeletalMesh(WeaponItemData->_weaponMesh.Get());
	}
}

void ACWCharacterBase::ReadScroll(UCWItemData* InItemData)
{
	UE_LOG(LogCWCharacter, Log, TEXT("ReadScroll"));
}
