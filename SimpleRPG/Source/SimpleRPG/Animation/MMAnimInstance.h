// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MMAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UMMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMMAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class ACharacter* _owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UCharacterMovementComponent* _movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FVector _velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float _groundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 _bIsIdle : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 _bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 _bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float _movingThreshold;
	 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float _jumpingThreshold;
};
