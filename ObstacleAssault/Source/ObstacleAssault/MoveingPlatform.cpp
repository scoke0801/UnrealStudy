// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveingPlatform.h"

// Sets default values
AMoveingPlatform::AMoveingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMoveingPlatform::BeginPlay()
{
	Super::BeginPlay();

	_startLocation = GetActorLocation();
}

// Called every frame
void AMoveingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MovePlatform(DeltaTime);
	
	RotatePlatform(DeltaTime);
}

void AMoveingPlatform::MovePlatform(float InDeltaTime)
{
	if(ShouldPlatformReturn())
	{
		FVector MoveDirection = _platformVelocity.GetSafeNormal();
		_startLocation = _startLocation + MoveDirection * _moveDistance;
		SetActorLocation(_startLocation);
		
		_platformVelocity *= -1.0f;
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += _platformVelocity * InDeltaTime;
		SetActorLocation(CurrentLocation);
	}
}

void AMoveingPlatform::RotatePlatform(float InDeltaTime)
{
	AddActorLocalRotation(_rotationVelocity * InDeltaTime);
}

bool AMoveingPlatform::ShouldPlatformReturn() const
{
	return GetMoveDistance() > _moveDistance;
}

float AMoveingPlatform::GetMoveDistance() const
{
	return FVector::Dist(_startLocation, GetActorLocation());
}

