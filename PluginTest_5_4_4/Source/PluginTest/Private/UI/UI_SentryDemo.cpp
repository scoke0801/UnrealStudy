// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_SentryDemo.h"

#include "SentryEvent.h"
#include "SentryId.h"
#include "SentryLibrary.h"
#include "SentrySubsystem.h"
#include "SentryDataTypes.h"

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

		// 여기에 실제 유저 정보가 들어가기만 하면 되나??
		USentryUser* User = USentryLibrary::CreateSentryUser("UserTest@gmail.com", "userTest", "PluginTest", "127.0.0.1", Data);

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
	if(USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>())
	{
		if(SentrySubsystem->IsEnabled())
		{
			FConfigureScopeDelegate ConfigureScopeDelegate;
			ConfigureScopeDelegate.BindDynamic(this, &UUI_SentryDemo::OnConfigureScope);
			
			SentrySubsystem->CaptureMessageWithScope(TEXT("Scoped Message Comes In!"), ConfigureScopeDelegate, ESentryLevel::Info);
			UE_LOG(LogTemp, Display, TEXT("Capture Message"));
		}
	}
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
	USentryEvent* Event = NewObject<USentryEvent>();
	if(nullptr == Event)
	{
		return;
	}

	Event->SetMessage("Message for Scoped event");
	if(USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>())
	{
		if(SentrySubsystem->IsEnabled())
		{
			FConfigureScopeDelegate ConfigureScopeDelegate;
			ConfigureScopeDelegate.BindDynamic(this, &UUI_SentryDemo::OnEventConfigureScope);
			USentryId* Id = SentrySubsystem->CaptureEventWithScope(Event, ConfigureScopeDelegate);

			SentrySubsystem->CaptureUserFeedbackWithParams(Id, "test@sentry.com", "Wow! What an event has just occured!", "TestUser");
		}
	}
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

void UUI_SentryDemo::OnConfigureScope(USentryScope* InScope)
{
	TMap<FString, FString> TagMap;
	TagMap.Emplace("Test", "Test");
	
	InScope->SetTags(TagMap);

	InScope->SetEnvironment("Production");

	InScope->SetLevel(ESentryLevel::Fatal);

	// Attachment
	{
		TArray<uint8> ByteArary = USentryLibrary::StringToBytesArray("Test Sentry attachment bytes");
		USentryAttachment* Attachment = USentryLibrary::CreateSentryAttachmentWithData(ByteArary, "DemoByteAttachment.txt", "application/octet-stream");

		InScope->AddAttachment(Attachment);
	}
}

void UUI_SentryDemo::OnEventConfigureScope(USentryScope* InScope)
{
	InScope->SetTagValue("Zero", "Dawn");
	InScope->SetLevel(ESentryLevel::Error);

	USentryLibrary::SaveStringToFile("Test Sentry attachment file content.", "DemoFileAttachment.txt");
	InScope->AddAttachment(USentryLibrary::CreateSentryAttachmentWithPath("DemoFileAttachment.txt", "application/octet-stream"));
}
