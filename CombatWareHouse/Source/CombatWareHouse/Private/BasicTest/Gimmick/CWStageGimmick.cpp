// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Gimmick/CWStageGimmick.h"
#include "BasicTest/Physics/CWCollision.h"
#include "BasicTest/Character/CWNonPlayer.h"
#include "BasicTest/Prop/CWItemBox.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACWStageGimmick::ACWStageGimmick()
{
	_stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = _stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Game/Environment/Stages/SM_SQUARE.SM_SQUARE"));
	if (StageMeshRef.Object)
	{
		_stage->SetStaticMesh(StageMeshRef.Object);
	}

	_stageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	_stageTrigger->SetBoxExtent(FVector(775.0f, 775.0, 300.0f));
	_stageTrigger->SetupAttachment(_stage);
	_stageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	_stageTrigger->SetCollisionProfileName(CPROFILE_CWTRIGGER);
	_stageTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACWStageGimmick::OnStageTriggerBeginOverlap);

	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Game/Environment/Props/SM_GATE.SM_GATE"));
	for (FName GateSocket : GateSockets)
	{
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		Gate->SetStaticMesh(GateMeshRef.Object);
		Gate->SetupAttachment(_stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		_gates.Add(GateSocket, Gate);

		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(_stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_CWTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACWStageGimmick::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket);

		_gateTriggers.Add(GateTrigger);
	}

	//// State Section
	_currentState = EStageState::READY;
	_stateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ACWStageGimmick::SetReady)));
	_stateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ACWStageGimmick::SetFight)));
	_stateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ACWStageGimmick::SetChooseReward)));
	_stateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ACWStageGimmick::SetChooseNext)));

	// Fight Section
	_opponentSpawnTime = 2.0f;
	_opponentClass = ACWNonPlayer::StaticClass();

	// Reward Section
	_rewardBoxClass = ACWItemBox::StaticClass();
	for (FName GateSocket : GateSockets)
	{
		FVector BoxLocation = _stage->GetSocketLocation(GateSocket) / 2;
		_rewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

void ACWStageGimmick::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);

	SetState(_currentState);
}

void ACWStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1);

	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(_stage->DoesSocketExist(SocketName));

	FVector NewLocation = _stage->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllObjects,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		GetWorld()->SpawnActor<ACWStageGimmick>(NewLocation, FRotator::ZeroRotator);
	}
}

void ACWStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void ACWStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : _rewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			ACWItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}

void ACWStageGimmick::OnOpponentDestroyed(AActor* InDestoyedActor)
{
	SetState(EStageState::REWARD);
}

void ACWStageGimmick::OnOpponentSpawn()
{
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;
	if (AActor* OpponentActor = GetWorld()->SpawnActor(_opponentClass, &SpawnLocation, &FRotator::ZeroRotator))
	{
		if (ACWNonPlayer* CWOpponentCharacter = Cast<ACWNonPlayer>(OpponentActor))
		{
			CWOpponentCharacter->OnDestroyed.AddDynamic(this, &ACWStageGimmick::OnOpponentDestroyed);
		}
	}
}

void ACWStageGimmick::SetState(const EStageState InNewState)
{
	_currentState = InNewState;

	if (auto action = _stateChangeActions.Find(InNewState))
	{
		action->_stageDelegate.ExecuteIfBound();
	}
}

void ACWStageGimmick::OpenAllGates()
{
	for (auto Gate : _gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void ACWStageGimmick::CloseAllGates()
{
	for (auto Gate : _gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void ACWStageGimmick::SetReady()
{
	_stageTrigger->SetCollisionProfileName(CPROFILE_CWTRIGGER);
	for (auto GateTrigger : _gateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	OpenAllGates();
}

void ACWStageGimmick::SetFight()
{
	_stageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : _gateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();


	GetWorld()->GetTimerManager().SetTimer(_opponentTimerHandle, this, &ACWStageGimmick::OnOpponentSpawn, _opponentSpawnTime, false);
}

void ACWStageGimmick::SetChooseReward()
{
	_stageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : _gateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();

	SpawnRewardBoxes();
}

void ACWStageGimmick::SetChooseNext()
{
	_stageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : _gateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_CWTRIGGER);
	}

	OpenAllGates();
}

void ACWStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : _rewardBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f);
		AActor* ItemActor = GetWorld()->SpawnActor(_rewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		ACWItemBox* RewardBoxActor = Cast<ACWItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ACWStageGimmick::OnRewardTriggerBeginOverlap);
			_rewardBoxes.Add(RewardBoxActor);
		}
	}
}
