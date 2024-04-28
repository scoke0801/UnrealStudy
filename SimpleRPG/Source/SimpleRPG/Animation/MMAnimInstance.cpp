// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MMAnimInstance.h"
#include "GameObject/Player/MMCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
UMMAnimInstance::UMMAnimInstance()
{
	_movingThreshold = 3.0f;
	_jumpingThreshold = 100.0f;
}

void UMMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	_owner = Cast<ACharacter>(GetOwningActor());
	if (_owner)
	{
		_movement = _owner->GetCharacterMovement();
	}
}

void UMMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (_movement)
	{
		_velocity = _movement->Velocity;
		_groundSpeed = _velocity.Size2D();
		_bIsIdle = _movement->IsFalling();
		_bIsJumping = (_bIsFalling & (_velocity.Z > _jumpingThreshold));
	}
}
