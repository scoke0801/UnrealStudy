// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PGCameraSystem.generated.h"

// 카메라 모드 열거형
UENUM(BlueprintType)
enum class EPGCameraMode : uint8
{
    ThirdPerson UMETA(DisplayName = "Third Person"),
    FirstPerson UMETA(DisplayName = "First Person"),
    FreeCamera UMETA(DisplayName = "Free Camera")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPLAYGROUND_API UPGCameraSystem : public UActorComponent
{
    GENERATED_BODY()

public:    
    // 생성자 및 초기화
    UPGCameraSystem();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 카메라 모드 전환 함수
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraMode(EPGCameraMode NewCameraMode);

    // 다음 카메라 모드로 순환 전환
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void CycleToNextCameraMode();

    // 현재 카메라 모드 반환
    UFUNCTION(BlueprintCallable, Category = "Camera")
    EPGCameraMode GetCurrentCameraMode() const { return CurrentCameraMode; }

    // 컴포넌트 초기화 함수
    void Initialize(USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

    // 카메라 입력 처리 함수
    void ProcessCameraZoom(float AxisValue);
    void ProcessFreeCameraMovement(const FVector& Direction);
    void ProcessFreeCameraRotation(const FRotator& Rotation);

    // 자유시점 카메라 활성화/비활성화 함수
    void EnableFreeCamera(bool bEnable);

protected:
    // 현재 카메라 모드
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    EPGCameraMode CurrentCameraMode;

    // 카메라 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    float ThirdPersonArmLength;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    FVector ThirdPersonSocketOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    FVector FirstPersonSocketOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    float FreeCameraMovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    float FreeCameraRotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    float MinZoomDistance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    float MaxZoomDistance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Settings")
    float ZoomStep;

private:
    // 참조 변수
    UPROPERTY()
    USpringArmComponent* SpringArmComponent;

    UPROPERTY()
    UCameraComponent* CameraComponent;

    // 자유시점 카메라 상태
    bool bFreeCameraActive;
    FVector FreeCameraLocation;
    FRotator FreeCameraRotation;
    FTransform OriginalCameraTransform;

    // 마지막 카메라 모드 (자유시점 모드 이전의 모드)
    EPGCameraMode LastCameraMode;

    // 카메라 모드별 설정 적용 함수
    void ApplyThirdPersonCameraSettings();
    void ApplyFirstPersonCameraSettings();
    void ApplyFreeCameraSettings();
};
