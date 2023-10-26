// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_CPLUSPLUS_API USearchTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	USearchTarget();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
