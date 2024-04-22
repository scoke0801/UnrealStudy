// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Player/MMCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameObject/Player/MMCharacterControlData.h"

// Sets default values
AMMCharacterBase::AMMCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0, -100.0f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> characterMeshRef(TEXT("SkeletalMesh'/Game/External/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'S"));
	if (characterMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(characterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstanceRef(TEXT("/Game/External/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	if (animInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMMCharacterControlData> ShoulderDataRef(TEXT("MMCharacterControlData'/Game/Data/CharacterData/MMC_Sholuder.MMC_Sholuder'"));
	if (ShoulderDataRef.Object)
	{
		_characterControlData.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
}

void AMMCharacterBase::SetCharacterControlData(const UMMCharacterControlData* CharacterControlData)
{
	// 폰
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// 캐릭터
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}
