// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/AI/BTTask_FindPatrolPos.h"
#include "BasicTest/Character/Interface/CWCharacterAIInterface.h"

#include "CWAICommon.h"
#include "AIController.h"
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControlingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	ICWCharacterAIInterface* AIPawn = Cast<ICWCharacterAIInterface>(ControlingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NaySystem = UNavigationSystemV1::GetNavigationSystem(ControlingPawn);
	if (nullptr == NaySystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = AIPawn->GetAIPatrolRadius();
	FNavLocation NextPatrolPos;

	if (NaySystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
