// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Player/MMCharacterBase.h"
#include "MMCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API AMMCharacterPlayer : public AMMCharacterBase
{
	GENERATED_BODY()
	
public:
	AMMCharacterPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 
	virtual void SetCharacterControlData(const class UMMCharacterControlData* CharacterControlData) override;
	 
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	 class USpringArmComponent* _cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _followCamera;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

};
