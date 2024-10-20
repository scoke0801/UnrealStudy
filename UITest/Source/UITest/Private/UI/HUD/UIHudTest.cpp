// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/UIHudTest.h"
#include "Management/UIManager.h"

#include "UI/Window/UIWindowBase.h"
#include "UI/Popup/UIPopupBase.h"
#include "UI/Etc/UINumText.h"

#include "Components/EditableTextBox.h" 
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"

void UUIHudTest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_editableTextBox1->OnTextCommitted.AddDynamic(this, &UUIHudTest::OnTextCommittedText_1);
	_editableTextBox2->OnTextCommitted.AddDynamic(this, &UUIHudTest::OnTextCommittedText_2);
}

void UUIHudTest::NativeConstruct()
{
	Super::NativeConstruct();

	//if (UUIWindowBase* Widget = UI()->OpenWIdget<UUIWindowBase>(TEXT("WindowBase")))
	//{
	//}
	//
	FindActorByBame("UIScoketActor");
}

void UUIHudTest::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (false == _isStarted)
	{
		return;
	}

	_elapsedTime += InDeltaTime;
	if (_elapsedTime > 0.1f)
	{
		_elapsedTime = 0.0f;
		AddTextValueWidget(FMath::RandRange(1000, 9999));
	}
}

void UUIHudTest::OnTextCommittedText_1(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (ETextCommit::Type::OnEnter == CommitMethod)
	{
		_isStarted = true;
	}
}

void UUIHudTest::OnTextCommittedText_2(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (ETextCommit::Type::OnEnter == CommitMethod)
	{
		_isStarted = false;
	}
}

void UUIHudTest::FindActorByBame(FName InTargetName)
{	
	// 모든 AActor를 찾고, 이름이 일치하는지 확인
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor->GetFName() == InTargetName || Actor->GetActorLabel() == InTargetName)
		{
			_targetActor = Actor;
			break;
		}
	}
}

FVector UUIHudTest::GetLocationOfActorSocket(FName InSocketName)
{
	USkeletalMeshComponent* SkeletalMesh = _targetActor->FindComponentByClass<USkeletalMeshComponent>(); 
	if (SkeletalMesh && SkeletalMesh->DoesSocketExist(InSocketName))
	{
		FTransform SocketTransform = SkeletalMesh->GetSocketTransform(InSocketName, RTS_World);
		
		return SocketTransform.GetLocation();
	}

	return FVector::ZeroVector;
}

void UUIHudTest::AddTextValueWidget(int32 InValue)
{

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (nullptr == PlayerController)
	{
		return;
	}
	
	// 3D 액터의 소켓 위치를 화면 좌표료 변환.
	FVector baseLocation = GetLocationOfActorSocket("UISocket");

	FVector2D screenLocation = FVector2D::ZeroVector;
	if (false == PlayerController->ProjectWorldLocationToScreen(baseLocation, screenLocation))
	{
		return;
	}

	// 랜덤 위치 보정.
	screenLocation = GenerateRandLocation(screenLocation);

	if(UUINumText* Widget = CreateWidget<UUINumText>(GetWorld(), _valueTextClass))
	{
		Widget->AddToViewport();

		Widget->SetValueText(InValue);

		// dpi 보정해서 위치 설정해주세요.
		Widget->SetPositionInViewport(screenLocation, true);

		Widget->OnRemoveNotify.BindUObject(this, &UUIHudTest::OnRemoveNotify);

		_locationMap.Emplace(Widget->GetUniqueID(), { Widget, screenLocation });
	}
}

void UUIHudTest::OnRemoveNotify(int32 InKeyValue)
{
	_locationMap.Remove(InKeyValue);
}

FVector2D UUIHudTest::GenerateRandLocation(const FVector2D InBaseLocation) const
{
	FVector2D RandomPosition = FVector2D::ZeroVector;
	bool bIsOverlapping = true;
	int32 tryCount = 0;
	
	float baseRandSize = 20;
	FVector2D randSize = { 50 * 3, 25 * 3};

	do
	{
		// 화면 내에서 랜덤한 위치 생성
		RandomPosition.X = InBaseLocation.X + FMath::FRandRange(-randSize.X * 0.5f, randSize.X * 0.5f);
		RandomPosition.Y = InBaseLocation.Y + FMath::FRandRange(0, randSize.Y);

		bIsOverlapping = false;

		for (auto Pair : _locationMap)
		{
			FVector2D ExistingPosition = Pair.Value._location;
			FVector2D ExistingSize = randSize;
			if (Pair.Value._widget.IsValid())
			{
				FVector2D size = Pair.Value._widget->GetCachedGeometry().GetLocalSize();
				if (size.IsZero() == false)
				{
					ExistingSize = size;
				}
			}

			if (FMath::Abs(RandomPosition.X - ExistingPosition.X) < ExistingSize.X  &&
				FMath::Abs(RandomPosition.Y - ExistingPosition.Y) < ExistingSize.Y )
			{
				bIsOverlapping = true;
				break;
			}

		}

		++tryCount;
	} while (bIsOverlapping && tryCount < 100);  // 겹치지 않을 때까지 반복

	return RandomPosition;
}
