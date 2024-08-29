// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CWCharacterBase.generated.h"

class UCWCharacterControlData;

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
	class ACWCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, UCWCharacterControlData*> CharacterControlManager;

public:
	// Sets default values for this character's properties
	ACWCharacterBase();


protected:
	virtual void SetCharacterControlData(const UCWCharacterControlData* InCharacterControlData);
};
