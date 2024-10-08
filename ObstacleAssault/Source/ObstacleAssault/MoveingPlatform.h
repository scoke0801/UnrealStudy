// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveingPlatform.generated.h"

UCLASS()
class OBSTACLEASSAULT_API AMoveingPlatform : public AActor
{
	GENERATED_BODY()

private:
	FVector _startLocation;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	FVector _platformVelocity = FVector(100,0,0);

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float _moveDistance = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation Platform")
	FRotator _rotationVelocity = FRotator::ZeroRotator;
	
public:	
	// Sets default values for this actor's properties
	AMoveingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void MovePlatform(float InDeltaTIme);
	void RotatePlatform(float InDeltaTime);

	bool ShouldPlatformReturn() const;
	float GetMoveDistance() const;
};
