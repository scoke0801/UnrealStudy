// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWStageGimmick.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ACWNonPlayer;
class ACWItemBox;

DECLARE_DELEGATE(FOnStageChangedDelegate);

USTRUCT()
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	
	explicit FStageChangedDelegateWrapper() { }
	explicit FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : _stageDelegate(InDelegate) {}

	FOnStageChangedDelegate _stageDelegate;
};

UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT,
};

UCLASS()
class ACWStageGimmick : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> _stage;
	
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> _stageTrigger;

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UStaticMeshComponent>> _gates;

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UBoxComponent>> _gateTriggers;

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> _stateChangeActions;

	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState _currentState;
protected:
	UPROPERTY(VisibleAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACWNonPlayer> _opponentClass;

	UPROPERTY(VisibleAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float _opponentSpawnTime = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACWItemBox> _rewardBoxClass;

	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<ACWItemBox>> _rewardBoxes;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 _currentStageNum;

private:
	FTimerHandle _opponentTimerHandle;

	TMap<FName, FVector> _rewardBoxLocations;
public:	
	// Sets default values for this actor's properties
	ACWStageGimmick();
	
protected:
	virtual void OnConstruction(const FTransform& InTransform) override;

protected:
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

protected:
	UFUNCTION()
	void OnOpponentDestroyed(AActor* InDestoyedActor);
	void OnOpponentSpawn();

public:
	FORCEINLINE int32 GetStageNum() const { return _currentStageNum; }
	FORCEINLINE void SetStageNum(int32 InNewStageNum) { _currentStageNum = InNewStageNum; }

protected:
	void SetState(const EStageState InNewState);
private:
	void OpenAllGates();
	void CloseAllGates();

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

	void SpawnRewardBoxes();
};
