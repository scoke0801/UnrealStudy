// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CWPlayerControllerBase.generated.h"

class UCWUIHUDIngame;
/**
 * 
 */
UCLASS()
class ACWPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<UCWUIHUDIngame> _HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<UCWUIHUDIngame> _HUDWIdget;

public:
	ACWPlayerControllerBase();

public:
	virtual void BeginPlay() override;

	
};
