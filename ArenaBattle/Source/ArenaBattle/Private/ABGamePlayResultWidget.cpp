// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGamePlayResultWidget.h"
#include "Components/TextBlock.h"
#include "ABGameState.h"


void UABGamePlayResultWidget::NativeConstruct()
{
	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));

	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Mission Complete") : TEXT("Mission Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));

	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}

void UABGamePlayResultWidget::BindGameState(AABGameState* GameState)
{
	CurrentGameState = GameState;
}
