// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
//	Quater
};

UCLASS()
class SIMPLERPG_API AMMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMMCharacterBase();

protected:
	virtual void SetCharacterControlData(const class UMMCharacterControlData* CharacterControlData);

	UPROPERTY()
	TMap<ECharacterControlType, class UMMCharacterControlData*> _characterControlData;
};
	