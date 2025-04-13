// PGTicker.h
#pragma once

#include "CoreMinimal.h"
#include "PGWidgetBase.h"
#include "PGTicker.generated.h"

USTRUCT(BlueprintType)
struct FPGTickerMessage
{
    GENERATED_BODY()

    // 메시지 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticker")
    FText Message;
    
    // 메시지 아이콘 (옵션)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticker")
    UTexture2D* Icon;
    
    // 메시지 표시 시간 (초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticker")
    float DisplayTime;
    
    // 메시지 타입 (커스텀 스타일링용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticker")
    FName MessageType;

    FPGTickerMessage()
        : Icon(nullptr)
        , DisplayTime(5.0f)
        , MessageType(NAME_None)
    {
    }
};

/**
 * UPGTicker - 짧은 알림 메시지를 위한 위젯 클래스
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGTicker : public UPGWidgetBase
{
    GENERATED_BODY()

public:
    UPGTicker(const FObjectInitializer& ObjectInitializer);

    // 메시지 추가 함수
    UFUNCTION(BlueprintCallable, Category = "Ticker")
    void AddMessage(const FText& Message, UTexture2D* Icon = nullptr, float DisplayTime = 5.0f, FName MessageType = NAME_None);
    
    // 메시지 구조체로 추가
    UFUNCTION(BlueprintCallable, Category = "Ticker")
    void AddTickerMessage(const FPGTickerMessage& TickerMessage);
    
    // 모든 메시지 제거
    UFUNCTION(BlueprintCallable, Category = "Ticker")
    void ClearAllMessages();

    // 위젯 오버라이드
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
    // UUserWidget 오버라이드
    virtual void NativeConstruct() override;
    
    // 메시지 큐
    UPROPERTY(BlueprintReadOnly, Category = "Ticker")
    TArray<FPGTickerMessage> MessageQueue;
    
    // 현재 표시 중인 메시지
    UPROPERTY(BlueprintReadOnly, Category = "Ticker")
    FPGTickerMessage CurrentMessage;
    
    // 현재 메시지 표시 타이머
    float CurrentMessageTimer;
    
    // 메시지가 표시 중인지 여부
    bool bIsShowingMessage;
    
    // 다음 메시지 표시
    UFUNCTION(BlueprintCallable, Category = "Ticker")
    void ShowNextMessage();
    
    // 메시지 위젯 업데이트
    UFUNCTION(BlueprintImplementableEvent, Category = "Ticker")
    void UpdateMessageWidget(const FPGTickerMessage& Message);
    
    // 메시지 컨테이너
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Ticker")
    class UPanelWidget* MessageContainer;
    
    // 메시지 텍스트 위젯
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Ticker")
    class UTextBlock* MessageText;
    
    // 메시지 아이콘 위젯
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Ticker")
    class UImage* MessageIcon;
};