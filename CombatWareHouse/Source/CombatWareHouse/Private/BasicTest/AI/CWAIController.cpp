// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/AI/CWAIController.h"

#include "CWAICommon.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ACWAIController::ACWAIController()
	:Super()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackBoardRef(TEXT("/Game/AI/BB_Monster.BB_Monster"));
	if (BlackBoardRef.Object)
	{
		_blackBoard = BlackBoardRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/AI/BT_Monster.BT_Monster"));
	if (BehaviorTreeRef.Object)
	{
		_behaviorTree = BehaviorTreeRef.Object;
	}
}

void ACWAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(_blackBoard, BlackboardPtr))
	{
		BlackboardPtr->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation()); 

		bool RunResult = RunBehaviorTree(_behaviorTree);
		ensure(RunResult);
	}
}

void ACWAIController::StopAI()
{
	if (UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BTComponent->StopTree();
	}
}

void ACWAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
