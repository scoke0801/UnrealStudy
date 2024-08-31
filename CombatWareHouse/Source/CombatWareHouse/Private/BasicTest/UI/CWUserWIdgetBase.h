// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWUserWIdgetBase.generated.h"

/**
 * 
 */
UCLASS()
class UCWUserWIdgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> _owningActor;


public:
	FORCEINLINE void SetOwningActor(AActor* InActor) { _owningActor = InActor; }

};
