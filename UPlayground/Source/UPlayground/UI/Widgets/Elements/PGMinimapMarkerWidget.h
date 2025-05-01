#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGMinimapMarkerWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGMinimapMarkerWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPGMinimapMarkerWidget(const FObjectInitializer& ObjectInitializer);
    
    // 마커 설정
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void SetMarkerData(FName InMarkerID, UTexture2D* MarkerIcon, const FLinearColor& MarkerColor);
    
    // 마커 ID 가져오기
    UFUNCTION(BlueprintPure, Category = "Minimap")
    FName GetMarkerID() const { return MarkerID; }
    
protected:
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> MarkerImage;
    
    // 마커 정보
    UPROPERTY(BlueprintReadOnly, Category = "Minimap")
    FName MarkerID;
    
    UPROPERTY(BlueprintReadOnly, Category = "Minimap")
    FLinearColor MarkerColor;
};