// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGCameraSystem.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UPGCameraSystem::UPGCameraSystem()
{
    PrimaryComponentTick.bCanEverTick = true;

    // 기본값 설정
    CurrentCameraMode = EPGCameraMode::ThirdPerson;
    LastCameraMode = EPGCameraMode::ThirdPerson;
    
    // 카메라 설정 초기화
    ThirdPersonArmLength = 300.0f;
    ThirdPersonSocketOffset = FVector(0.0f, 0.0f, 0.0f);
    FirstPersonSocketOffset = FVector(0.0f, 0.0f, 50.0f); // 머리 위치에 맞게 조정
    
    FreeCameraMovementSpeed = 500.0f;
    FreeCameraRotationSpeed = 5.0f;
    
    MinZoomDistance = 150.0f;
    MaxZoomDistance = 800.0f;
    ZoomStep = 20.0f;
    
    bFreeCameraActive = false;
}

void UPGCameraSystem::BeginPlay()
{
    Super::BeginPlay();
    
    // 컴포넌트가 생성될 때 초기 카메라 모드 적용
    SetCameraMode(CurrentCameraMode);
}

void UPGCameraSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 자유시점 카메라 모드에서만 매 틱마다 처리
    if (CurrentCameraMode == EPGCameraMode::FreeCamera && bFreeCameraActive && SpringArmComponent && CameraComponent)
    {
        // 필요한 경우 여기서 자유시점 카메라 업데이트
    }
}

void UPGCameraSystem::Initialize(USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
    SpringArmComponent = InSpringArm;
    CameraComponent = InCamera;
    
    if (SpringArmComponent && CameraComponent)
    {
        // 현재 카메라 모드 설정을 적용
        SetCameraMode(CurrentCameraMode);
    }
}

void UPGCameraSystem::SetCameraMode(EPGCameraMode NewCameraMode)
{
    if (!SpringArmComponent || !CameraComponent)
        return;
    
    // 자유시점 모드를 나갈 때 원래 변환 복원
    if (CurrentCameraMode == EPGCameraMode::FreeCamera && NewCameraMode != EPGCameraMode::FreeCamera)
    {
        EnableFreeCamera(false);
    }
    
    // 이전 모드 저장 (자유 카메라가 아닌 경우만)
    if (NewCameraMode != EPGCameraMode::FreeCamera)
    {
        LastCameraMode = NewCameraMode;
    }
    
    // 새 모드 설정
    CurrentCameraMode = NewCameraMode;
    
    // 모드별 설정 적용
    switch (CurrentCameraMode)
    {
        case EPGCameraMode::ThirdPerson:
            ApplyThirdPersonCameraSettings();
            break;
            
        case EPGCameraMode::FirstPerson:
            ApplyFirstPersonCameraSettings();
            break;
            
        case EPGCameraMode::FreeCamera:
            ApplyFreeCameraSettings();
            break;
    }
}

void UPGCameraSystem::CycleToNextCameraMode()
{
    EPGCameraMode NextMode;
    
    switch (CurrentCameraMode)
    {
        case EPGCameraMode::ThirdPerson:
            NextMode = EPGCameraMode::FirstPerson;
            break;
            
        case EPGCameraMode::FirstPerson:
            NextMode = EPGCameraMode::FreeCamera;
            break;
            
        case EPGCameraMode::FreeCamera:
            NextMode = EPGCameraMode::ThirdPerson;
            break;
            
        default:
            NextMode = EPGCameraMode::ThirdPerson;
            break;
    }
    
    SetCameraMode(NextMode);
}

void UPGCameraSystem::ProcessCameraZoom(float AxisValue)
{
    if (!SpringArmComponent || CurrentCameraMode != EPGCameraMode::ThirdPerson)
        return;
    
    // 줌 인/아웃 처리
    float NewTargetArmLength = SpringArmComponent->TargetArmLength - (AxisValue * ZoomStep);
    SpringArmComponent->TargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomDistance, MaxZoomDistance);
}

void UPGCameraSystem::ProcessFreeCameraMovement(const FVector& Direction)
{
    if (!SpringArmComponent || !CameraComponent || CurrentCameraMode != EPGCameraMode::FreeCamera || !bFreeCameraActive)
        return;
    
    // 자유 시점 카메라 이동 처리
    FVector CameraForward = CameraComponent->GetForwardVector();
    FVector CameraRight = CameraComponent->GetRightVector();
    FVector CameraUp = CameraComponent->GetUpVector();
    
    // 카메라 방향으로 이동 벡터 계산
    FVector MovementDelta = (CameraForward * Direction.X + CameraRight * Direction.Y + CameraUp * Direction.Z) * FreeCameraMovementSpeed * GetWorld()->GetDeltaSeconds();
    
    // 카메라 위치 업데이트
    SpringArmComponent->SetWorldLocation(SpringArmComponent->GetComponentLocation() + MovementDelta);
}

void UPGCameraSystem::ProcessFreeCameraRotation(const FRotator& Rotation)
{
    if (!SpringArmComponent || !CameraComponent || CurrentCameraMode != EPGCameraMode::FreeCamera || !bFreeCameraActive)
        return;
    
    // 자유 시점 카메라 회전 처리
    FRotator NewRotation = SpringArmComponent->GetRelativeRotation() + Rotation * FreeCameraRotationSpeed;
    SpringArmComponent->SetRelativeRotation(NewRotation);
}

void UPGCameraSystem::EnableFreeCamera(bool bEnable)
{
    if (!SpringArmComponent || !CameraComponent)
        return;
    
    if (bEnable && !bFreeCameraActive)
    {
        // 자유 시점 카메라 활성화
        bFreeCameraActive = true;
        
        // 현재 카메라 변환 저장
        OriginalCameraTransform.SetLocation(SpringArmComponent->GetComponentLocation());
        OriginalCameraTransform.SetRotation(SpringArmComponent->GetComponentQuat());
        
        // Spring Arm 속성 조정
        SpringArmComponent->bInheritPitch = false;
        SpringArmComponent->bInheritYaw = false;
        SpringArmComponent->bInheritRoll = false;
        SpringArmComponent->bDoCollisionTest = false;
    }
    else if (!bEnable && bFreeCameraActive)
    {
        // 자유 시점 카메라 비활성화
        bFreeCameraActive = false;
        
        // 이전 카메라 모드로 복구
        if (LastCameraMode == EPGCameraMode::FreeCamera)
            LastCameraMode = EPGCameraMode::ThirdPerson;
        
        CurrentCameraMode = LastCameraMode;
        
        if (CurrentCameraMode == EPGCameraMode::ThirdPerson)
            ApplyThirdPersonCameraSettings();
        else
            ApplyFirstPersonCameraSettings();
    }
}

void UPGCameraSystem::ApplyThirdPersonCameraSettings()
{
    if (!SpringArmComponent || !CameraComponent)
        return;
    
    // 3인칭 카메라 설정
    SpringArmComponent->TargetArmLength = ThirdPersonArmLength;
    SpringArmComponent->SocketOffset = ThirdPersonSocketOffset;
    SpringArmComponent->bUsePawnControlRotation = true;
    CameraComponent->bUsePawnControlRotation = false;
    
    // 카메라 부착 설정
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        // 스프링 암 위치 및 회전 설정
        SpringArmComponent->SetupAttachment(OwnerPawn->GetRootComponent());
        SpringArmComponent->bInheritPitch = true;
        SpringArmComponent->bInheritYaw = true;
        SpringArmComponent->bInheritRoll = true;
        SpringArmComponent->bDoCollisionTest = true;
    }
}

void UPGCameraSystem::ApplyFirstPersonCameraSettings()
{
    if (!SpringArmComponent || !CameraComponent)
        return;
    
    // 1인칭 카메라 설정
    SpringArmComponent->TargetArmLength = 0.0f;
    SpringArmComponent->SocketOffset = FirstPersonSocketOffset;
    SpringArmComponent->bUsePawnControlRotation = true;
    CameraComponent->bUsePawnControlRotation = false;
    
    // 카메라 부착 설정
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        // 스프링 암 위치 및 회전 설정
        SpringArmComponent->SetupAttachment(OwnerPawn->GetRootComponent());
        SpringArmComponent->bInheritPitch = true;
        SpringArmComponent->bInheritYaw = true;
        SpringArmComponent->bInheritRoll = true;
        SpringArmComponent->bDoCollisionTest = false; // 1인칭에서는 충돌 검사 비활성화
    }
}

void UPGCameraSystem::ApplyFreeCameraSettings()
{
    if (!SpringArmComponent || !CameraComponent)
        return;
    
    // 자유 시점 카메라 설정
    EnableFreeCamera(true);
}
