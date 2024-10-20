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

		_locationMap.Emplace(Widget->GetUniqueID(), screenLocation);
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
	float randSize = 20;

	do
	{
		// ȭ�� ������ ������ ��ġ ����
		RandomPosition.X = InBaseLocation.X + FMath::FRandRange(-randSize * 0.5f, randSize * 0.5f);
		RandomPosition.Y = InBaseLocation.Y + FMath::FRandRange(0, randSize);

		bIsOverlapping = false;

		for (auto Pair : _locationMap)
		{
			FVector2D ExistingPosition = Pair.Value;

			if (FMath::Abs(RandomPosition.X - ExistingPosition.X) < (randSize) / 2 &&
				FMath::Abs(RandomPosition.Y - ExistingPosition.Y) < (randSize) / 2)
			{
				bIsOverlapping = true;
				break;
			}

			float Dist = FVector2D::Distance(Pair.Value, RandomPosition);
		}

		++tryCount;

		// ���� ũ�� Ű���
		randSize = FMath::Min(randSize + 2, 100);
	} while (bIsOverlapping && tryCount < 100);  // ��ġ�� ���� ������ �ݺ�

	return RandomPosition;
}
