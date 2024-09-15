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
	// Move Platform forwards
	
	// Get Current Location
	FVector CurrentLocation = GetActorLocation();

	// add vector to that location
	CurrentLocation += _platformVelocity * InDeltaTime;
	
	// Set te location
	SetActorLocation(CurrentLocation);

	// Send platform back if gone too far
	// check how far we've moved
	_distanceMoved = FVector::Distance(CurrentLocation, _startLocation);
	
	// Reverse Direction of motion if gone too far
	if(_distanceMoved >= _moveDistance)
	{
		{
			FString Name = GetName();
			float OverShoot = _distanceMoved - _moveDistance;
			UE_LOG(LogTemp, Display, TEXT("%s OverShoot : %f"), *Name ,OverShoot);
		}
		
		FVector MoveDirection = _platformVelocity.GetSafeNormal();
		_startLocation = _startLocation + MoveDirection * _moveDistance;
		SetActorLocation(_startLocation);
		
		_platformVelocity *= -1.0f;
	}
}

void AMoveingPlatform::RotatePlatform(float InDeltaTime)
{
	UE_LOG(LogTemp, Display, TEXT("%s Rotating..."), *GetName());
}

