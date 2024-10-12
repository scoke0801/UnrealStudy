// Fill out your copyright notice in the Description page of Project Settings.


#include "Management/UIManager.h"


#include "UObject/SoftObjectPath.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

//TObjectPtr<UGameWidgetBase> UIManager::GetOrCreateWidget(const FString WidgetName)
//{
//	static ConstructorHelpers::FObjectFinder<UDataTable> DTWidgetPath(TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData"));
//
//	if (TObjectPtr<UDataTable> WidgetPathTable = DTWidgetPath.Object)
//	{
//		TArray<FBSWIdgetPath*> Rows;
//		WidgetPathTable->GetAllRows<FBSWIdgetPath>(TEXT("GetAllRows"), Rows);
//
//		for (FBSWIdgetPath* Row : Rows)
//		{
//			if (WidgetName == Row->WidgetName)
//			{
//				FStringClassReference ClassRef(Row->WidgetName);
//				if (UClass* WidgetClass = ClassRef.TryLoadClass<UGameWidgetBase>())
//				{
//					CreateWidget<UGameWidgetBase
//				}
//
//				//CreateWidget(, Row->Path, Row->WidgetName);
//				
//				//HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
//			}
//		}
//	}
//
//	return nullptr;
//}
