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
	
	// Move Platform forwards
	{
		// Get Current Location
		FVector CurrentLocation = GetActorLocation();

		// add vector to that location
		CurrentLocation += _platformVelocity * DeltaTime;
	
		// Set te location
		SetActorLocation(CurrentLocation);

		// Send platform back if gone too far
		// check how far we've moved
		_distanceMoved = FVector::Distance(CurrentLocation, _startLocation);
	
		// Reverse Direction of motion if gone too far
	}
}

