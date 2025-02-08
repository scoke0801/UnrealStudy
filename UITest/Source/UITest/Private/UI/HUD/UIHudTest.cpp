// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/UIHudTest.h"

#include "UI/Etc/UIScrollWidget.h"
#include "UI/Etc/UINumText.h"
#include "Management/UIManager.h"

#include "UI/Window/UIWindowBase.h"
#include "UI/Popup/UIPopupBase.h"
#include "UI/Etc/UINumText.h"

#include "Components/EditableTextBox.h" 
#include "Components/SkeletalMeshComponent.h"
#include "Components/ListView.h"

#include "Kismet/GameplayStatics.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

void UUIHudTest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_editableTextBox1->OnTextCommitted.AddDynamic(this, &UUIHudTest::OnTextCommittedText_1);
	_editableTextBox2->OnTextCommitted.AddDynamic(this, &UUIHudTest::OnTextCommittedText_2);

	_listView->OnListViewScrolled().AddUObject(this, &UUIHudTest::OnListViewScrolledInternal);
}
	
void UUIHudTest::NativeConstruct()
{
	Super::NativeConstruct();

	//if (UUIWindowBase* Widget = UI()->OpenWIdget<UUIWindowBase>(TEXT("WindowBase")))
	//{
	//}
	//

	TArray<UUIScrollWIdgetItem*> listItems;

	for (int i = 1; i <= 30; ++i)
	{
		UUIScrollWIdgetItem* newItem = NewObject<UUIScrollWIdgetItem>(this);
		newItem->_data = i;
		listItems.Add(newItem);
	}
	_listView->SetListItems(listItems);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_listView->NavigateToIndex(listItems.Num() * 0.5f);
}

void UUIHudTest::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// if (false == _isStarted)
	// {
	// 	return;
	// }
	//
	// _elapsedTime += InDeltaTime;
	// if (_elapsedTime > 0.1f)
	// {
	// 	_elapsedTime = 0.0f;
	// 	AddTextValueWidget(FMath::RandRange(1000, 9999));
	// }
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

void UUIHudTest::OnListViewScrolledInternal(float InOffset, float DistanceRemaining)
{
	int32 Offset = InOffset;
	UE_LOG(LogTemp,Display, TEXT("OnListViewScrolledInternal - InOffset: %f, Offset: %d"), InOffset, Offset);

	
}

void UUIHudTest::FindActorByBame(FName InTargetName)
{	
	// ��� AActor�� ã��, �̸��� ��ġ�ϴ��� Ȯ��
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
	
	// 3D ������ ���� ��ġ�� ȭ�� ��ǥ�� ��ȯ.
	FVector baseLocation = GetLocationOfActorSocket("UISocket");

	FVector2D screenLocation = FVector2D::ZeroVector;
	if (false == PlayerController->ProjectWorldLocationToScreen(baseLocation, screenLocation))
	{
		return;
	}

	// ���� ��ġ ����.
	screenLocation = GenerateRandLocation(screenLocation);

	if(UUINumText* Widget = CreateWidget<UUINumText>(GetWorld(), _valueTextClass))
	{
		Widget->AddToViewport();

		Widget->SetValueText(InValue);

		// dpi �����ؼ� ��ġ �������ּ���.
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
		// ȭ�� ������ ������ ��ġ ����
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
	} while (bIsOverlapping && tryCount < 100);  // ��ġ�� ���� ������ �ݺ�

	return RandomPosition;
}
