// PGTicker.cpp
#include "PGTicker.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"

UPGTicker::UPGTicker(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentMessageTimer(0.0f)
    , bIsShowingMessage(false)
{
    // 알림은 화면 상단에 표시되므로 낮은 Z-오더를 가짐
    ZOrder = 50;
}

void UPGTicker::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 메시지 컨테이너 초기 상태 설정
    if (MessageContainer)
    {
        MessageContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UPGTicker::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 현재 메시지가 표시 중인 경우 타이머 업데이트
    if (bIsShowingMessage)
    {
        CurrentMessageTimer -= InDeltaTime;
        
        // 타이머가 끝나면 다음 메시지 표시
        if (CurrentMessageTimer <= 0.0f)
        {
            if (MessageQueue.Num() > 0)
            {
                ShowNextMessage();
            }
            else
            {
                // 더 이상 메시지가 없으면 컨테이너 숨김
                bIsShowingMessage = false;
                if (MessageContainer)
                {
                    MessageContainer->SetVisibility(ESlateVisibility::Hidden);
                }
            }
        }
    }
    else if (MessageQueue.Num() > 0)
    {
        // 대기 중인 메시지가 있고 현재 표시 중인 메시지가 없는 경우 다음 메시지 표시
        ShowNextMessage();
    }
}

void UPGTicker::AddMessage(const FText& Message, UTexture2D* Icon, float DisplayTime, FName MessageType)
{
    FPGTickerMessage NewMessage;
    NewMessage.Message = Message;
    NewMessage.Icon = Icon;
    NewMessage.DisplayTime = DisplayTime;
    NewMessage.MessageType = MessageType;
    
    AddTickerMessage(NewMessage);
}

void UPGTicker::AddTickerMessage(const FPGTickerMessage& TickerMessage)
{
    // 메시지 큐에 새 메시지 추가
    MessageQueue.Add(TickerMessage);
    
    // 현재 표시 중인 메시지가 없으면 바로 표시
    if (!bIsShowingMessage && IsWidgetActive())
    {
        ShowNextMessage();
    }
}

void UPGTicker::ClearAllMessages()
{
    // 모든 메시지 제거
    MessageQueue.Empty();
    bIsShowingMessage = false;
    
    if (MessageContainer)
    {
        MessageContainer->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UPGTicker::ShowNextMessage()
{
    if (MessageQueue.Num() > 0)
    {
        // 큐에서 다음 메시지 가져오기
        CurrentMessage = MessageQueue[0];
        MessageQueue.RemoveAt(0);
        
        // 타이머 설정
        CurrentMessageTimer = CurrentMessage.DisplayTime;
        bIsShowingMessage = true;
        
        // 메시지 위젯 업데이트
        if (MessageText)
        {
            MessageText->SetText(CurrentMessage.Message);
        }
        
        if (MessageIcon)
        {
            if (CurrentMessage.Icon)
            {
                MessageIcon->SetBrushFromTexture(CurrentMessage.Icon);
                MessageIcon->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                MessageIcon->SetVisibility(ESlateVisibility::Hidden);
            }
        }
        
        // 컨테이너 표시
        if (MessageContainer)
        {
            MessageContainer->SetVisibility(ESlateVisibility::Visible);
        }
        
        // 블루프린트 구현 이벤트 호출
        UpdateMessageWidget(CurrentMessage);
    }
}