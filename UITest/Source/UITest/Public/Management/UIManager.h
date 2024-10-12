// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Data/DTWidgetPath.h"
#include "UI/GameWidgetBase.h"

UPROPERTY(EBlueprintType)
enum class EUIType
{
	None,

	HUD,

	Window,

	Popup,

	Max,
};
#define UPTR TObjectPtr
/**
 * 
 */
class UITEST_API UIManager
{
private:
	TMap<EUIType, TArray<TObjectPtr<UGameWidgetBase>>> _uiMap;

public:
	UIManager();
	~UIManager();

public:
	template<typename WidgetType>
	TObjectPtr<UGameWidgetBase> GetOrCreateWidget(const FString WidgetName);
};

template<typename WidgetType>
inline TObjectPtr<UGameWidgetBase> UIManager::GetOrCreateWidget(const FString WidgetName)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DTWidgetPath(TEXT("/Game/Data/DTWidgetPath.DTWidgetPath"));

	if (TObjectPtr<UDataTable> WidgetPathTable = DTWidgetPath.Object)
	{
		TArray<FBSWIdgetPath*> Rows;
		WidgetPathTable->GetAllRows<FBSWIdgetPath>(TEXT("GetAllRows"), Rows);

		for (FBSWIdgetPath* Row : Rows)
		{
			if (WidgetName == Row->WidgetName)
			{
				FStringClassReference ClassRef(Row->WidgetName);
				if (UClass* WidgetClass = ClassRef.TryLoadClass<WidgetType>())
				{
					if (WidgetType* Widget = CreateWidget<WidgetType>(this, WidgetClass))
					{
						Widget->AddToViewport();
					}	
				}
			}
		}
	}

	return nullptr;

}
