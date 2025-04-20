#include "PGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Combat/PGCombatComponent.h"
#include "../Abilities/PGAbilityComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Input/PGEnhancedInputComponent.h"
#include "../Input/PGInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APGPlayerCharacter::APGPlayerCharacter()
{
    // 카메라 붐 생성
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;
    
    // 팔로우 카메라 생성
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    
    // 캐릭터 기본 회전 설정
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;
}

void APGPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 로컬 플레이어인 경우에만 입력 매핑 컨텍스트 추가
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }
}

void APGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Cast to enhanced input component
    PGInputComponent = Cast<UPGEnhancedInputComponent>(PlayerInputComponent);
    
    if (PGInputComponent && InputConfig)
    {
        // Movement
        if (InputConfig->MoveAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->MoveAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Move);
        }
        
        if (InputConfig->LookAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->LookAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Look);
        }
        
        if (InputConfig->JumpAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->JumpAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::JumpAction);
        }
        
        if (InputConfig->SprintAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->SprintAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Sprint);
        }
        
        // Combat
        if (InputConfig->PrimaryAttackAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->PrimaryAttackAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::PrimaryAttack);
        }
        
        if (InputConfig->SecondaryAttackAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->SecondaryAttackAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::SecondaryAttack);
        }
        
        if (InputConfig->TargetAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->TargetAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::SelectTarget);
        }
        
        if (InputConfig->ClearTargetAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->ClearTargetAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::ClearTarget);
        }
        
        // Abilities
        if (InputConfig->Ability1Action)
        {
            PGInputComponent->BindActionSafe(InputConfig->Ability1Action, ETriggerEvent::Triggered, this, &APGPlayerCharacter::UseAbility1);
        }
        
        if (InputConfig->Ability2Action)
        {
            PGInputComponent->BindActionSafe(InputConfig->Ability2Action, ETriggerEvent::Triggered, this, &APGPlayerCharacter::UseAbility2);
        }
        
        if (InputConfig->Ability3Action)
        {
            PGInputComponent->BindActionSafe(InputConfig->Ability3Action, ETriggerEvent::Triggered, this, &APGPlayerCharacter::UseAbility3);
        }
        
        if (InputConfig->Ability4Action)
        {
            PGInputComponent->BindActionSafe(InputConfig->Ability4Action, ETriggerEvent::Triggered, this, &APGPlayerCharacter::UseAbility4);
        }
        
        // Interaction
        if (InputConfig->InteractAction)
        {
            PGInputComponent->BindActionSafe(InputConfig->InteractAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::InteractInput);
        }
    }
}

void APGPlayerCharacter::SetCameraDistance(float Distance)
{
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = Distance;
    }
}

void APGPlayerCharacter::SelectTarget()
{
    if (CombatComponent)
    {
        CombatComponent->SelectTarget();
    }
}

void APGPlayerCharacter::ClearTarget()
{
    if (CombatComponent)
    {
        CombatComponent->ClearTarget();
    }
}

void APGPlayerCharacter::Interact(APGBaseCharacter* Interactor)
{
    // 플레이어가 상호작용을 받았을 때의 처리
    Super::Interact(Interactor);
    
    // 플레이어 특화된 상호작용 처리
    UE_LOG(LogTemp, Log, TEXT("Player Character Interacted"));
}

void APGPlayerCharacter::InteractInput(const FInputActionValue& Value)
{
    // 플레이어가 상호작용 키를 눌렀을 때 근처 오브젝트와 상호작용
    UE_LOG(LogTemp, Log, TEXT("Player pressed Interact button"));
    
    // 여기서 주변 상호작용 가능한 오브젝트를 탐색하고 상호작용을 시작할 수 있음
    // 예: 가장 가까운 NPC 찾기, 아이템 줍기, 문 열기 등
}

void APGPlayerCharacter::Move(const FInputActionValue& Value)
{
    // Movement input
    const FVector2D MovementVector = Value.Get<FVector2D>();
    
    if (Controller != nullptr)
    {
        // Forward/Backward movement
        if (MovementVector.Y != 0.0f)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(Direction, MovementVector.Y);
        }
        
        // Right/Left movement
        if (MovementVector.X != 0.0f)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            AddMovementInput(Direction, MovementVector.X);
        }
    }
}

void APGPlayerCharacter::Look(const FInputActionValue& Value)
{
    // Look input
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    
    if (Controller != nullptr)
    {
        // Up/Down looking
        AddControllerPitchInput(LookAxisVector.Y);
        
        // Left/Right looking
        AddControllerYawInput(LookAxisVector.X);
    }
}

void APGPlayerCharacter::JumpAction(const FInputActionValue& Value)
{
    ACharacter::Jump();
}

void APGPlayerCharacter::Sprint(const FInputActionValue& Value)
{
    // 사용자 정의 움직임 컴포넌트를 통한 달리기 구현
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        // 여기서는 기본 언리얼 움직임 컴포넌트를 사용
        // 실제 구현에서는 UPGCharacterMovementComponent를 사용할 수 있음
        const float SprintSpeedMultiplier = 1.5f;
        MovementComponent->MaxWalkSpeed = 600.0f * SprintSpeedMultiplier;
    }
}

void APGPlayerCharacter::PrimaryAttack(const FInputActionValue& Value)
{
    if (CombatComponent)
    {
        CombatComponent->PrimaryAttack();
    }
}

void APGPlayerCharacter::SecondaryAttack(const FInputActionValue& Value)
{
    if (CombatComponent)
    {
        CombatComponent->SecondaryAttack();
    }
}

void APGPlayerCharacter::UseAbility1(const FInputActionValue& Value)
{
    if (AbilityComponent)
    {
        // 실제 구현에서는 특정 능력 ID를 사용
        AbilityComponent->ActivateAbility(FName("Ability1"));
    }
}

void APGPlayerCharacter::UseAbility2(const FInputActionValue& Value)
{
    if (AbilityComponent)
    {
        AbilityComponent->ActivateAbility(FName("Ability2"));
    }
}

void APGPlayerCharacter::UseAbility3(const FInputActionValue& Value)
{
    if (AbilityComponent)
    {
        AbilityComponent->ActivateAbility(FName("Ability3"));
    }
}

void APGPlayerCharacter::UseAbility4(const FInputActionValue& Value)
{
    if (AbilityComponent)
    {
        AbilityComponent->ActivateAbility(FName("Ability4"));
    }
}
