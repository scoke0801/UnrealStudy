// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWItemBox.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UCWItemData;

UCLASS()
class ACWItemBox : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> _triggerBoxComp;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UStaticMeshComponent> _mesh;

	UPROPERTY(VIsibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> _effect;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<UCWItemData> _item;

public:	
	// Sets default values for this actor's properties
	ACWItemBox();
	
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

};
