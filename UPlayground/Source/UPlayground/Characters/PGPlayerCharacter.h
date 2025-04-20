#pragma once

#include "CoreMinimal.h"
#include "PGBaseCharacter.h"
#include "InputActionValue.h"
#include "PGPlayerCharacter.generated.h"

/**
 * 플레이어 캐릭터 클래스
 */
UCLASS()
class UPLAYGROUND_API APGPlayerCharacter : public APGBaseCharacter
{
    GENERATED_BODY()
    
public:
    APGPlayerCharacter();
    
    // 입력 처리 설정
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    // 카메라 컨트롤
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraDistance(float Distance);
    
    // 타겟 선택
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SelectTarget();
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ClearTarget();
    
    // 상호작용
    virtual void Interact(APGBaseCharacter* Interactor) override;

    // 상호작용 입력 핸들러
    void InteractInput(const FInputActionValue& Value);
    
protected:
    // BeginPlay
    virtual void BeginPlay() override;
    
    // 카메라 관련 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* CameraBoom;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;
    
    // 향상된 입력 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
    class UPGEnhancedInputComponent* PGInputComponent;
    
    // 입력 맵핑 컨텍스트
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;
    
    // 입력 설정
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UPGInputConfig* InputConfig;
    
    // 입력 액션 핸들러
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void JumpAction(const FInputActionValue& Value);
    void Sprint(const FInputActionValue& Value);
    void PrimaryAttack(const FInputActionValue& Value);
    void SecondaryAttack(const FInputActionValue& Value);
    void UseAbility1(const FInputActionValue& Value);
    void UseAbility2(const FInputActionValue& Value);
    void UseAbility3(const FInputActionValue& Value);
    void UseAbility4(const FInputActionValue& Value);
};
