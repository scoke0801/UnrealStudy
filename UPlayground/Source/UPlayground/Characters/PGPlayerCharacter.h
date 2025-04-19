// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGBaseCharacter.h"
#include "EnhancedInput/Public/InputActionValue.h" // EnhancedInput 관련
#include "PGPlayerCharacter.generated.h"

// 플레이어 유형 (로컬/리모트 구분)
UENUM(BlueprintType)
enum class EPGPlayerType : uint8
{
    LocalPlayer   UMETA(DisplayName = "Local Player"),
    RemotePlayer  UMETA(DisplayName = "Remote Player")
};

UCLASS()
class UPLAYGROUND_API APGPlayerCharacter : public APGBaseCharacter
{
    GENERATED_BODY()

public:
    APGPlayerCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // 상호작용 오버라이드
    virtual void Interact(APGBaseCharacter* Interactor) override;

protected:
    // 플레이어 타입 (로컬 또는 리모트)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
    EPGPlayerType PlayerType;

    // 플레이어 정보
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString PlayerName;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    int32 PlayerLevel;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Stats")
    int32 Experience;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Stats")
    int32 ExperienceForNextLevel;

    // 플레이어 특수 능력치
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    int32 Strength;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    int32 Dexterity;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    int32 Intelligence;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    int32 Constitution;

    // 플레이어 고유 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* FollowCamera;

    // EnhancedInput 액션 매핑
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* InteractAction;

    // 플레이어 메서드
    UFUNCTION(BlueprintCallable, Category = "Player")
    virtual void AddExperience(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Player")
    virtual void CheckLevelUp();

    UFUNCTION(BlueprintCallable, Category = "Player")
    virtual void AttemptInteraction();

    UFUNCTION(BlueprintCallable, Category = "Player")
    virtual bool IsLocalPlayer() const;

    // 리모트 플레이어 관련 메서드
    UFUNCTION(BlueprintCallable, Category = "Player")
    virtual void SyncWithRemotePlayer();

    // 서버-클라이언트 RPC 함수
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerMoveCharacter(const FVector& Direction);
    
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerInteract();

    // 델리게이트 선언
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceGained, int32, Amount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryOpened);
    
    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Player Events")
    FOnExperienceGained OnExperienceGained;
    
    UPROPERTY(BlueprintAssignable, Category = "Player Events")
    FOnInventoryOpened OnInventoryOpened;

private:
    // 상호작용 대상 찾기
    APGBaseCharacter* FindInteractableTarget();
};