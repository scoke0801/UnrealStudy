// PGWidgetBase.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGWidgetBase.generated.h"

UENUM(BlueprintType)
enum class EPGWidgetState : uint8
{
    Inactive,       // 비활성화 상태
    Appearing,      // 표시 중
    Active,         // 활성화 상태
    Disappearing,   // 숨김 중
};

/**
 * UPGWidgetBase - 모든 UI 위젯의 기본 클래스
 * 공통 기능과 생명주기 관리를 담당합니다.
 */
UCLASS(Abstract, Blueprintable)
class UPLAYGROUND_API UPGWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    UPGWidgetBase(const FObjectInitializer& ObjectInitializer);

    // 위젯 표시 함수
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void ShowWidget();

    // 위젯 숨김 함수
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void HideWidget();

    // 애니메이션 완료 후 위젯 파괴
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void DestroyWidget();

    // 위젯 초기화 (데이터 설정 등)
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void InitializeWidget();

    // 위젯 현재 상태 반환
    UFUNCTION(BlueprintPure, Category = "Widget")
    EPGWidgetState GetWidgetState() const { return CurrentState; }

    // 위젯이 활성화 상태인지 확인
    UFUNCTION(BlueprintPure, Category = "Widget")
    bool IsWidgetActive() const { return CurrentState == EPGWidgetState::Active; }

protected:
    // UUserWidget 오버라이드
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    // 표시 애니메이션 시작
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void PlayShowAnimation();

    // 숨김 애니메이션 시작
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void PlayHideAnimation();

    // 애니메이션 완료 콜백
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void OnShowAnimationFinished();
    
    UFUNCTION(BlueprintCallable, Category = "Widget")
    virtual void OnHideAnimationFinished();

    // 현재 위젯 상태
    UPROPERTY(BlueprintReadOnly, Category = "Widget")
    EPGWidgetState CurrentState;

    // 표시/숨김 애니메이션
    UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Animation")
    UWidgetAnimation* ShowAnimation;

    UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim), Category = "Animation")
    UWidgetAnimation* HideAnimation;

    // 위젯 Z-오더(Order) 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
    int32 ZOrder;

    // ESC 키로 닫기 가능 여부
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
    bool bCloseOnEsc;
};