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
	
	SetActorLocation(_vectorVar);
}

// Called every frame
void AMoveingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector LocalVector = _vectorVar;
	
	LocalVector.Z += 1;
	_vectorVar.Y += 1;
	
	SetActorLocation(_vectorVar);
}

