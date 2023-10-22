// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("/Script/Engine.DataTable'/Game/Data/StatTable.StatTable'"));

	if (DATA.Succeeded())
	{
		MyStats = DATA.Object;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DATA file does not exist."));
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("MyGameInstance: %d"), GetStatData(1)->Attack);
}

FMyChracterData* UMyGameInstance::GetStatData(int32 Level)
{
	return MyStats->FindRow<FMyChracterData>(*FString::FromInt(Level), TEXT(""));
}
