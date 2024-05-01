// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/Level/MMLevelChanager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMMLevelChanager::UMMLevelChanager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMMLevelChanager::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Map/Title.Title"));
}


// Called every frame
void UMMLevelChanager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

