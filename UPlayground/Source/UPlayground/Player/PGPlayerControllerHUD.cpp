#include "Player/PGPlayerControllerHUD.h"
#include "UI/Components/PGHUDComponent.h"

APGPlayerControllerHUD::APGPlayerControllerHUD()
{
    // HUD 컴포넌트 생성
    HUDComponent = CreateDefaultSubobject<UPGHUDComponent>(TEXT("HUDComponent"));
}

void APGPlayerControllerHUD::BeginPlay()
{
    Super::BeginPlay();
}

UPGHUDComponent* APGPlayerControllerHUD::GetHUDComponent() const
{
    return HUDComponent;
}