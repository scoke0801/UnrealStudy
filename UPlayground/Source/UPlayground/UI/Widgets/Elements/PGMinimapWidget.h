#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "PGMinimapWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGMinimapWidget : public UPGHUDBaseWidget
{
    GENERATED_BODY()
    
public:
    UPGMinimapWidget(const FObjectInitializer& ObjectInitializer);
    
    // 위젯 업데이트 오버라이드
    virtual void UpdateWidget() override;
    
    // 미니맵 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void SetMinimapTexture(UTexture2D* MapTexture);
    
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void SetPlayerPosition(const FVector2D& Position, float Rotation);
    
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void SetMinimapZoom(float ZoomLevel);
    
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void AddMarker(FName MarkerID, const FVector2D& Position, UTexture2D* MarkerIcon, const FLinearColor& MarkerColor);
    
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void UpdateMarkerPosition(FName MarkerID, const FVector2D& Position);
    
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void RemoveMarker(FName MarkerID);
    
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void ClearAllMarkers();
    
protected:
    // 틱 업데이트
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> MinimapImage;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> PlayerMarker;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> MarkersContainer;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UButton> ZoomInButton;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UButton> ZoomOutButton;
    
    // 마커 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    TSubclassOf<class UPGMinimapMarkerWidget> MarkerWidgetClass;
    
    // 줌 설정
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float MinZoom = 0.5f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float MaxZoom = 2.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float ZoomStep = 0.25f;
    
    // 현재 줌 레벨
    UPROPERTY(BlueprintReadOnly, Category = "Minimap")
    float CurrentZoom = 1.0f;
    
    // 마커 관리
    UPROPERTY()
    TMap<FName, TObjectPtr<class UPGMinimapMarkerWidget>> MarkerWidgets;
    
    // 버튼 이벤트 핸들러
    UFUNCTION()
    void OnZoomInClicked();
    
    UFUNCTION()
    void OnZoomOutClicked();
    
    // 초기 설정
    virtual void NativeConstruct() override;
};