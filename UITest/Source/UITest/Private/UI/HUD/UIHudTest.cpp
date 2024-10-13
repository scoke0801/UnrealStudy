// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/UIHudTest.h"
#include "Management/UIManager.h"
#include "UI/Window/UIWindowBase.h"
#include "UI/Popup/UIPopupBase.h"
 
void UUIHudTest::NativeConstruct()
{
	Super::NativeConstruct();

	///*UIManager* uiManager = GetGameInstance()->GetSubsystem<UIManager>();
	if (UUIWindowBase* Widget = UI()->OpenWIdget<UUIWindowBase>(TEXT("WindowBase")))
	{
	}
	
	 
	//FString WidgetName = TEXT("WindowBase");
	//if (UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/DTWidgetPath.DTWidgetPath'")))
	//{
	//	TArray<FBSWIdgetPath*> Rows;
	//	DataTable->GetAllRows<FBSWIdgetPath>(TEXT("GetAllRows"), Rows);

	//	for (FBSWIdgetPath* Row : Rows)
	//	{
	//		if (WidgetName == Row->WidgetName)
	//		{
	//			FStringClassReference ClassRef(Row->Path.ToString());
	//			if (UClass* WidgetClass = ClassRef.TryLoadClass<UUIWindowBase>())
	//			{
	//				if (UUIWindowBase* Widget = CreateWidget<UUIWindowBase>(GetWorld()->GetFirstPlayerController(), WidgetClass))
	//				{
	//					Widget->AddToViewport();
	//				}
	//			}
	//		}
	//	}
	//}
}
