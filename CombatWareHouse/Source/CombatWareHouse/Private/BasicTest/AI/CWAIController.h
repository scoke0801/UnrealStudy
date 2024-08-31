// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CWAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACWAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UBlackboardData> _blackBoard;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> _behaviorTree;

public:
	ACWAIController();

public:
	void RunAI();
	void StopAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
