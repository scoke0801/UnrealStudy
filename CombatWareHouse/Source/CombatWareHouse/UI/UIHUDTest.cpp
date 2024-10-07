// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatWareHouse/UI/UIHUDTest.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
DEFINE_LOG_CATEGORY(LogJH);

void UUIHUDTest::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	_startButton->OnClicked.AddDynamic(this, &UUIHUDTest::OnClickedStartButton);
	_endButton->OnClicked.AddDynamic(this, &UUIHUDTest::OnClicedEndButtion);

	_editableText->OnTextCommitted.AddDynamic(this, &UUIHUDTest::OnTextCommited);
}

void UUIHUDTest::Process()
{
	if(false ==_isRunning)
	{
		return;
	}

	double CurrentTime = FPlatformTime::Seconds();
	if(CurrentTime -_prevProcessTime < 0.032f)
	{
		UE_LOG(LogJH, Display, TEXT("Set Work For Next Tick: %f"), CurrentTime -_prevProcessTime);
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this](){Process();}));
		return;
	}
	_prevProcessTime = CurrentTime;

	UE_LOG(LogJH, Display, TEXT("ProcessTime: %f"), _prevProcessTime);
}

void UUIHUDTest::OnClickedStartButton()
{
	_isRunning = true;

	UE_LOG(LogJH, Display, TEXT("OnClickedStartButton()"));
	Process();
	Process();
	Process();
	Process();
}

void UUIHUDTest::OnClicedEndButtion()
{
	_isRunning = false;
	
	UE_LOG(LogJH, Display, TEXT("OnClicedEndButtion()"));
}

void UUIHUDTest::OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (ETextCommit::Type::OnCleared == CommitMethod)
	{
		_editableText->SetFocus();
	}
}
