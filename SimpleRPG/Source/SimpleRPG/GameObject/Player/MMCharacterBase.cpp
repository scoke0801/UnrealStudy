// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Player/MMCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameObject/Player/MMCharacterControlData.h"
#include "Engine/DataTable.h"
#include "GameData/MMCharacterStat.h"
#include "Algo/Transform.h"

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

	static ConstructorHelpers::FClassFinder<UAnimInstance> animInstanceRef(TEXT("/Game/Blueprint/Animation/ABP_MMCharacter.ABP_MMCharacter_C"));
	if (animInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(animInstanceRef.Class);
	}
	static ConstructorHelpers::FObjectFinder<UMMCharacterControlData> ShoulderDataRef(TEXT("MMCharacterControlData'/Game/Data/CharacterData/MMC_Sholuder.MMC_Sholuder'"));
	if (ShoulderDataRef.Object)
	{
		_characterControlData.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	TArray< FMMCharacterStat*> ValueArray;
	TArray<FMMCharacterStat> CharacterStatTable;
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Data/CharacterData/MMCharacterStatTable.MMCharacterStatTable"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTablePtr = DataTableRef.Object;
		check(DataTablePtr->GetRowMap().Num() > 0);
		 
		TArray<FName> RowNames = DataTablePtr->GetRowNames();
		for (int i = 0; i < DataTablePtr->GetRowMap().Num(); ++i)
		{
			FMMCharacterStat* stat = DataTablePtr->FindRow<FMMCharacterStat>(RowNames[i], TEXT(""));
			ValueArray.Push(stat);
		}  
	}

	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTablePtr = DataTableRef.Object;
		check(DataTablePtr->GetRowMap().Num() > 0);

		TArray<uint8*> innerValueArray;
		DataTablePtr->GetRowMap().GenerateValueArray(innerValueArray);
		Algo::Transform(innerValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FMMCharacterStat*>(Value);
			}
		);
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
