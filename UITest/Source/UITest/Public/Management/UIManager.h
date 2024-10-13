// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ManagerBase.h"
#include "Data/DTWidgetPath.h"
#include "Common/UICommon.h"
#include "UI/GameWidgetBase.h"

/**
 * 
 */
class UITEST_API UIManager : public UManagerBase
{ 
private:
	TMap<EUIType, TArray<TObjectPtr<UGameWidgetBase>>> _uiMap;

public:
	UIManager();
	virtual ~UIManager();
public:
	template<typename WidgetType>
	WidgetType* OpenWIdget(const FString WidgetName);
};

template<typename WidgetType>
inline WidgetType* UIManager::OpenWIdget(const FString WidgetName)
{
	if (UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/DTWidgetPath.DTWidgetPath'")))
	{
		TArray<FBSWIdgetPath*> Rows;
		DataTable->GetAllRows<FBSWIdgetPath>(TEXT("GetAllRows"), Rows);
		 
		for (FBSWIdgetPath* Row : Rows)
		{
			if (WidgetName == Row->WidgetName)
			{
				FStringClassReference ClassRef(Row->Path.ToString());
				if (UClass* WidgetClass = ClassRef.TryLoadClass<WidgetType>())
				{
					if (WidgetType* Widget = CreateWidget<WidgetType>(GetWorld()->GetFirstPlayerController(), WidgetClass))
					{
						Widget->AddToViewport();
						return Widget;
					}
				}
			}
		}
	}
	return nullptr;
}

#define UI() GetGameInstance()->GetSubsystem<UIManager>()
