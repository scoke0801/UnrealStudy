// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CWCharacterAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class UCWCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCWCharacterAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<ACharacter> _owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> _movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FVector _velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float _groundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float _movingThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float _jumpingThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 _bIsIdle : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 _bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 _bIsJumping : 1;
	
protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float InDeltaSeconds) override;

public:
	bool IsCanAttack() const;
};
