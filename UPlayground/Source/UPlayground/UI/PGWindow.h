// PGWindow.h
#pragma once

#include "CoreMinimal.h"
#include "PGWidgetBase.h"
#include "PGWindow.generated.h"

/**
 * UPGWindow - 주요 게임 창(인벤토리, 캐릭터 정보 등)을 위한 위젯 클래스
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGWindow : public UPGWidgetBase
{
    GENERATED_BODY()

public:
    UPGWindow(const FObjectInitializer& ObjectInitializer);

    // 창 제목 설정/가져오기
    UFUNCTION(BlueprintCallable, Category = "Window")
    void SetWindowTitle(const FText& NewTitle);
    
    UFUNCTION(BlueprintPure, Category = "Window")
    FText GetWindowTitle() const { return WindowTitle; }
    
    // 창 이동 가능 여부 설정/가져오기
    UFUNCTION(BlueprintCallable, Category = "Window")
    void SetDraggable(bool bInDraggable);
    
    UFUNCTION(BlueprintPure, Category = "Window")
    bool IsDraggable() const { return bDraggable; }
    
    // 창 크기 조절 가능 여부 설정/가져오기
    UFUNCTION(BlueprintCallable, Category = "Window")
    void SetResizable(bool bInResizable);
    
    UFUNCTION(BlueprintPure, Category = "Window")
    bool IsResizable() const { return bResizable; }

protected:
    // UUserWidget 오버라이드
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 창 제목
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window")
    FText WindowTitle;
    
    // 창 이동 관련 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window")
    bool bDraggable;
    
    // 창 크기 조절 관련 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window")
    bool bResizable;
    
    // 창 이동 상태 추적
    bool bIsDragging;
    FVector2D DragOffset;
    
    // 타이틀바 위젯
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Window")
    class UPanelWidget* TitleBar;
    
    // 닫기 버튼
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Window")
    class UButton* CloseButton;
    
    // 타이틀 텍스트 위젯
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Window")
    class UTextBlock* TitleText;
    
    // 창 이동 시작 함수
    UFUNCTION()
    void StartDrag(const FVector2D& InMousePosition);
    
    // 창 이동 종료 함수
    UFUNCTION()
    void EndDrag();
    
    // 닫기 버튼 클릭 핸들러
    UFUNCTION()
    void OnCloseButtonClicked();
};