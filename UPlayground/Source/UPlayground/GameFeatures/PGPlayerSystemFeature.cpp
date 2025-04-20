#include "PGPlayerSystemFeature.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

UPGPlayerSystemFeature::UPGPlayerSystemFeature()
{
    // 기본 생성자
}

void UPGPlayerSystemFeature::OnGameFeatureActivating()
{
    Super::OnGameFeatureActivating();
    
    // 이 클래스의 로직 간소화 (필요에 따라 나중에 구현)
    UE_LOG(LogTemp, Log, TEXT("PGPlayerSystemFeature Activating"));
}

void UPGPlayerSystemFeature::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    // 이 클래스의 로직 간소화 (필요에 따라 나중에 구현)
    UE_LOG(LogTemp, Log, TEXT("PGPlayerSystemFeature Deactivating"));
    
    Super::OnGameFeatureDeactivating(Context);
}