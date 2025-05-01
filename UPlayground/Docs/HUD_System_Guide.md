# UPlayground HUD 시스템 사용 가이드

## 개요
본 문서는 UPlayground 프로젝트에 추가된 HUD 시스템의 사용법을 설명합니다. 이 시스템은 게임 내에서 다양한 UI 요소를 관리하는 모듈화된 프레임워크를 제공합니다.

## 기능
- 캐릭터 정보 표시 (체력, 경험치, 버프/디버프)
- 퀵슬롯 시스템
- 메뉴 시스템
- 미니맵
- UI 요소의 동적 표시/숨김
- 게임 상태에 따른 UI 전환
- EnhancedInput을 통한 UI 상호작용

## 주요 컴포넌트

### 1. UI 매니저 시스템
- `UPGUIManager`: UI 시스템 전체를 관리하는 매니저
- `UPGHUDManager`: HUD 요소를 관리하는 매니저

### 2. HUD 컴포넌트
- `UPGHUDComponent`: 플레이어 컨트롤러에 추가하여 HUD 시스템을 활성화

### 3. UI 위젯 클래스
- `UPGHUDBaseWidget`: 모든 HUD 위젯의 기본 클래스
- `UPGHUDContainerWidget`: 모든 UI 요소를 포함하는 컨테이너 위젯
- `UPGCharacterInfoWidget`: 캐릭터 정보 (체력, 경험치, 버프/디버프) 표시
- `UPGQuickSlotWidget`: 퀵슬롯 관리
- `UPGMinimapWidget`: 미니맵 표시
- `UPGMenuWidget`: 게임 메뉴 관리

### 4. 설정 데이터
- `UPGHUDConfigDataAsset`: HUD 요소의 레이아웃 및 설정 정의
- `UPGQuickSlotInputConfig`: 퀵슬롯 및 UI 관련 입력 설정

## 설정 방법

### 1. UI 매니저 등록
게임 피쳐 플러그인을 통해 UI 매니저를 자동으로 등록할 수 있습니다.

```cpp
// 게임 피쳐 데이터 에셋에 추가
UPGUIFeature* UIFeature = NewObject<UPGUIFeature>(this);
UIFeature->UIManagerClass = UPGUIManager::StaticClass();

// HUD 요소 등록
FPGHUDElementConfig CharacterInfoConfig;
CharacterInfoConfig.ElementID = TEXT("CharacterInfo");
CharacterInfoConfig.WidgetClass = UPGCharacterInfoWidget::StaticClass();
CharacterInfoConfig.Layer = EPGHUDLayer::GameLayer;
CharacterInfoConfig.DefaultVisibility = EPGHUDVisibility::Visible;
UIFeature->HUDElementConfigs.Add(CharacterInfoConfig);

GameFeatureActions.Add(UIFeature);
```

### 2. 플레이어 컨트롤러 설정
```cpp
// 플레이어 컨트롤러에 HUD 컴포넌트 추가
YourPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // 기존 컨트롤러에 HUD 컴포넌트 추가
    UPGHUDComponent* HUDComp = NewObject<UPGHUDComponent>(this);
    HUDComp->RegisterComponent();
}

// 또는 제공된 HUD 지원 컨트롤러를 상속
class UPLAYGROUND_API AMyPlayerController : public APGPlayerControllerHUD
{
    GENERATED_BODY()
    
    // 추가 기능 구현...
};
```

### 3. HUD 설정 데이터 생성
1. Content Browser에서 우클릭 → Create → Miscellaneous → Data Asset
2. UPGHUDConfigDataAsset 클래스 선택
3. 다음 설정 추가:
   - HUD 요소 (캐릭터 정보, 퀵슬롯, 미니맵, 메뉴 등)
   - 각 요소의 위치 (코너 배치)
   - 레이어 (배경, 게임, 전경, 모달)
   - 기본 가시성

### 4. 블루프린트로 UI 위젯 생성
1. 각 UI 요소 (UPGCharacterInfoWidget, UPGQuickSlotWidget 등)에 대한 블루프린트 위젯 생성
2. 필요한 UI 구성 요소 (텍스트, 이미지, 버튼 등) 추가
3. Variable 패널에서 C++ 클래스에 정의된 변수와 동일한 이름으로 위젯 변수 생성 및 연결

## 사용 예제

### 캐릭터 정보 업데이트
```cpp
// 체력 표시 업데이트
void AMyCharacter::UpdateHealth(float CurrentHealth, float MaxHealth)
{
    APGPlayerControllerHUD* PC = Cast<APGPlayerControllerHUD>(GetController());
    if (PC && PC->GetHUDComponent())
    {
        PC->GetHUDComponent()->UpdateCharacterHP(CurrentHealth, MaxHealth);
    }
}

// 버프 추가
void AMyCharacter::ApplyBuff(FName BuffID, UTexture2D* Icon, float Duration)
{
    APGPlayerControllerHUD* PC = Cast<APGPlayerControllerHUD>(GetController());
    if (PC && PC->GetHUDComponent())
    {
        FPGBuffData BuffData;
        BuffData.BuffID = BuffID;
        BuffData.BuffName = FText::FromString(TEXT("힘 증가"));
        BuffData.BuffDescription = FText::FromString(TEXT("공격력 +10%"));
        BuffData.BuffIcon = Icon;
        BuffData.BuffType = EPGBuffType::Positive;
        BuffData.Duration = Duration;
        BuffData.RemainingTime = Duration;
        
        PC->GetHUDComponent()->AddCharacterBuff(BuffData);
    }
}
```

### 퀵슬롯 관리
```cpp
// 아이템을 퀵슬롯에 등록
void AMyInventoryComponent::AddItemToQuickSlot(UPGItem* Item, int32 SlotIndex)
{
    APGPlayerControllerHUD* PC = Cast<APGPlayerControllerHUD>(GetOwner()->GetController());
    if (PC && PC->GetHUDComponent())
    {
        FPGQuickSlotItemData ItemData;
        ItemData.ItemID = Item->GetItemID();
        ItemData.ItemName = Item->GetDisplayName();
        ItemData.ItemIcon = Item->GetIcon();
        ItemData.ItemType = Item->IsConsumable() ? EPGQuickSlotItemType::Consumable : EPGQuickSlotItemType::Equipment;
        ItemData.Quantity = Item->GetStackCount();
        
        PC->GetHUDComponent()->SetQuickSlotItem(SlotIndex, ItemData);
    }
}
```

### 메뉴 관리
```cpp
// 메뉴 토글
void AMyGameMode::ToggleGameMenu()
{
    APGPlayerControllerHUD* PC = Cast<APGPlayerControllerHUD>(GetWorld()->GetFirstPlayerController());
    if (PC && PC->GetHUDComponent())
    {
        UPGMenuWidget* MenuWidget = PC->GetHUDComponent()->GetMenuWidget();
        if (MenuWidget)
        {
            MenuWidget->ToggleMenu();
        }
    }
}
```

## 확장 방법

### 새 UI 요소 추가하기
1. `UPGHUDBaseWidget`을 상속하는 새 위젯 클래스 생성
2. 필요한 UI 기능 구현
3. HUD 설정 데이터에 새 요소 추가
4. 게임 피쳐나 HUD 컴포넌트를 통해 등록

```cpp
// 새 UI 요소 등록
FPGHUDElementConfig NewElementConfig;
NewElementConfig.ElementID = TEXT("NewElement");
NewElementConfig.WidgetClass = UMyNewWidget::StaticClass();
NewElementConfig.Layer = EPGHUDLayer::Foreground;
NewElementConfig.DefaultVisibility = EPGHUDVisibility::Visible;

HUDManager->RegisterHUDElement(NewElementConfig);
```

## 주의사항
- 모든 HUD 위젯은 `UPGHUDBaseWidget`을 상속해야 합니다.
- HUD 요소의 표시/숨김은 `UPGHUDComponent`나 `UPGHUDManager`를 통해 관리해야 합니다.
- 입력 처리는 `EnhancedInput` 시스템을 통해 구현됩니다.
- HUD 컨테이너 위젯은 모든 UI 요소의 부모이므로 적절한 레이아웃을 가져야 합니다.

## 파일 구조
```
Source/
  UPlayground/
    UI/
      Types/
        PGHUDTypes.h - UI 관련 타입 정의
      Managers/
        IPGIUIManager.h/.cpp - UI 매니저 인터페이스
        PGUIManager.h/.cpp - UI 시스템 총괄 매니저
        PGHUDManager.h/.cpp - HUD 관련 기능 관리
      Features/
        PGUIFeature.h/.cpp - UI 게임 피쳐 플러그인
      Components/
        PGHUDComponent.h/.cpp - HUD 컴포넌트
      Widgets/
        Base/
          PGHUDBaseWidget.h/.cpp - 모든 HUD 위젯의 기본 클래스
          PGHUDContainerWidget.h/.cpp - HUD 컨테이너(Root) 위젯
        Elements/
          PGCharacterInfoWidget.h/.cpp - 캐릭터 정보 표시
          PGBuffWidget.h/.cpp - 버프/디버프 표시
          PGQuickSlotWidget.h/.cpp - 퀵슬롯 기능
          PGQuickSlotItemWidget.h/.cpp - 개별 퀵슬롯 항목
          PGMinimapWidget.h/.cpp - 미니맵
          PGMinimapMarkerWidget.h/.cpp - 미니맵 마커
          PGMenuWidget.h/.cpp - 메뉴 시스템
          PGMenuButtonWidget.h/.cpp - 메뉴 버튼
      Data/
        PGHUDConfigDataAsset.h - HUD 설정 데이터 에셋
      Input/
        PGQuickSlotInputConfig.h - 퀵슬롯 입력 설정
    Player/
      PGPlayerControllerHUD.h/.cpp - HUD 지원 플레이어 컨트롤러
```

## 질문 및 문제 해결
- UI 요소가 표시되지 않는 경우, HUD 컴포넌트가 올바르게 등록되었는지 확인
- UI 요소가 올바른 레이어와 위치에 설정되었는지 확인
- 입력이 작동하지 않는 경우, EnhancedInput 설정을 확인