// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_SentryDemo.h"

#include "SentryEvent.h"
#include "SentryId.h"
#include "SentryLibrary.h"
#include "SentrySubsystem.h"

#include "Components/Button.h"
#include "Components/ComboBox.h"
#include "Components/ComboBoxString.h"

void UUI_SentryDemo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_Button_CaptureMessage->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnClickedCaptureMessage);
	_Button_CaptureMessageWithScope->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnClickedCaptureMessageWithScope);
	
	_Button_CaptureEvent->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnClickedCaptureEvent);
	_Button_CaptureEventWithScope->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnClickedCaptureEventWithScope);

	_Button_Crash->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnCLickedTerminate);
	
	_Button_Performance->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnClickedPerformance);
	_Button_Performance2->OnClicked.AddDynamic(this, &UUI_SentryDemo::OnClickedPerformance);
}

void UUI_SentryDemo::NativeConstruct()
{
	Super::NativeConstruct();

	USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>();
	if(nullptr == SentrySubsystem)
	{
		return;
	}

	{
		TMap<FString, FString> Data;
		Data.Emplace(TEXT("Engine"), TEXT("UE"));
	 
		SentrySubsystem->AddBreadcrumbWithParams(TEXT("Subsystem initialized"), TEXT("Sentry"), TEXT(""), Data, ESentryLevel::Warning);
	}
		
	{
		TMap<FString, FString> Data;
		Data.Emplace("Mood", "Serious");
		Data.Emplace("Look", "Good");
		
		USentryUser* User = USentryLibrary::CreateSentryUser("test@sentry.com", "123456789", "Demouser", "127.0.0.1", Data);

		SentrySubsystem->SetUser(User);
	}
}

void UUI_SentryDemo::OnClickedCaptureMessage()
{
	if(USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>())
	{
		if(SentrySubsystem->IsEnabled())
		{
			SentrySubsystem->CaptureMessage(TEXT("Yay! Here Comes the new Message..."));
			UE_LOG(LogTemp, Display, TEXT("Capture Message"));
		}
	}
}

void UUI_SentryDemo::OnClickedCaptureMessageWithScope()
{
}

void UUI_SentryDemo::OnClickedScopeConfigure()
{
}

void UUI_SentryDemo::OnClickedCaptureEvent()
{
	USentryEvent* Event = USentryEvent::CreateEventWithMessageAndLevel("Message For Common Event", ESentryLevel::Warning);
	if(nullptr == Event)
	{
		return;
	}
	
	if(USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>())
	{
		if(SentrySubsystem->IsEnabled())
		{
			USentryId* Id = SentrySubsystem->CaptureEvent(Event);

			SentrySubsystem->CaptureUserFeedbackWithParams(Id, "test@sentry.com", "Wow! What an event has just occured!", "TestUser");
		}
	}
	
}

void UUI_SentryDemo::OnClickedCaptureEventWithScope()
{
}

void UUI_SentryDemo::OnCLickedTerminate()
{
	volatile char *ptr = nullptr;
	*ptr += 1;
}

void UUI_SentryDemo::OnClickedPerformance()
{
}

void UUI_SentryDemo::OnClickedPerformance2()
{
}
