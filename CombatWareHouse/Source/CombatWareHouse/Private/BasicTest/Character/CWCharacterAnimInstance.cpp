// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCWCharacterAnimInstance::UCWCharacterAnimInstance()
{
	_movingThreshold = 3.0f;
	
}

void UCWCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (_owner = Cast<ACharacter>(GetOwningActor()))
	{
		_movement = _owner->GetCharacterMovement();
	}	
}

void UCWCharacterAnimInstance::NativeUpdateAnimation(float InDeltaSeconds)
{
	Super::NativeUpdateAnimation(InDeltaSeconds);

	if (_movement)
	{
		_velocity = _movement->Velocity;
		_groundSpeed = _velocity.Size2D();
		_bIsIdle = _groundSpeed < _movingThreshold;
		_bIsFalling = _movement->IsFalling();
		_bIsJumping = _bIsFalling & (_velocity.Z > _jumpingThreshold);
	}
}
