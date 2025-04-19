// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGPlayerCharacter.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"
#include "PGNPC.h"

APGPlayerCharacter::APGPlayerCharacter()
{
    // 네트워크 복제 활성화
    bReplicates = true;
    
    // 카메라 붐 생성
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // 팔로우 카메라 생성
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // 캐릭터 무브먼트 설정
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;

    // 플레이어 속성 초기화
    PlayerType = EPGPlayerType::RemotePlayer; // 기본값으로 리모트 플레이어 설정
    PlayerName = "Player";
    PlayerLevel = 1;
    Experience = 0;
    ExperienceForNextLevel = 1000;
    
    // RPG 능력치 초기화
    Strength = 10;
    Dexterity = 10;
    Intelligence = 10;
    Constitution = 10;
}

void APGPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어 체크 및 설정
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC && PC->IsLocalPlayerController())
    {
        PlayerType = EPGPlayerType::LocalPlayer;

        // EnhancedInput 매핑 컨텍스트 설정
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    // UE 5.4 API 호환성
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
}

void APGPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 플레이어 틱 로직
}

void APGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // EnhancedInput 설정
    // 구체적인 입력 처리는 요구사항에 따라 구현하지 않음
}

void APGPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 네트워크 복제 속성 설정
    DOREPLIFETIME(APGPlayerCharacter, PlayerType);
    DOREPLIFETIME(APGPlayerCharacter, PlayerName);
    DOREPLIFETIME(APGPlayerCharacter, PlayerLevel);
    DOREPLIFETIME(APGPlayerCharacter, Experience);
    DOREPLIFETIME(APGPlayerCharacter, ExperienceForNextLevel);
    DOREPLIFETIME(APGPlayerCharacter, Strength);
    DOREPLIFETIME(APGPlayerCharacter, Dexterity);
    DOREPLIFETIME(APGPlayerCharacter, Intelligence);
    DOREPLIFETIME(APGPlayerCharacter, Constitution);
}

void APGPlayerCharacter::Interact(APGBaseCharacter* Interactor)
{
    Super::Interact(Interactor);
    
    // 다른 플레이어와의 상호작용 로직
}

void APGPlayerCharacter::AddExperience(int32 Amount)
{
    Experience += Amount;
    
    // 경험치 획득 이벤트 발생
    OnExperienceGained.Broadcast(Amount);
    
    CheckLevelUp();
}

void APGPlayerCharacter::CheckLevelUp()
{
    if (Experience >= ExperienceForNextLevel)
    {
        Experience -= ExperienceForNextLevel;
        LevelUp();
        ExperienceForNextLevel = static_cast<int32>(ExperienceForNextLevel * 1.5f); // 다음 레벨업 경험치 증가
    }
}

void APGPlayerCharacter::AttemptInteraction()
{
    // 로컬 플레이어인 경우에만 상호작용 시도
    if (IsLocalPlayer())
    {
        APGBaseCharacter* Target = FindInteractableTarget();
        if (Target && Target->CanBeInteractedWith())
        {
            // 로컬에서 처리 후 서버로 전송
            ServerInteract();
            
            // 타겟과 상호작용
            Target->Interact(this);
        }
    }
}

bool APGPlayerCharacter::IsLocalPlayer() const
{
    return PlayerType == EPGPlayerType::LocalPlayer;
}

void APGPlayerCharacter::SyncWithRemotePlayer()
{
    // 리모트 플레이어 동기화 로직
}

bool APGPlayerCharacter::ServerMoveCharacter_Validate(const FVector& Direction)
{
    return true; // 간단한 검증
}

void APGPlayerCharacter::ServerMoveCharacter_Implementation(const FVector& Direction)
{
    // 서버에서의 이동 처리 로직
}

bool APGPlayerCharacter::ServerInteract_Validate()
{
    return true; // 간단한 검증
}

void APGPlayerCharacter::ServerInteract_Implementation()
{
    // 서버에서의 상호작용 처리 로직
    APGBaseCharacter* Target = FindInteractableTarget();
    if (Target && Target->CanBeInteractedWith())
    {
        Target->Interact(this);
    }
}

APGBaseCharacter* APGPlayerCharacter::FindInteractableTarget()
{
    // 플레이어 주변의 상호작용 가능한 대상 찾기
    // 여기서는 간단한 구조만 정의
    
    // 실제 구현에서는 트레이스나 오버랩을 통해 가장 가까운 상호작용 가능 대상을 찾음
    
    return nullptr; // 구체적인 구현은 제외
}