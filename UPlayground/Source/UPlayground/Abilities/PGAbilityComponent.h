#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGAbilityTypes.h"
#include "PGAbilityComponent.generated.h"

/**
 * 능력을 사용할 수 있는 엔티티를 위한 컴포넌트
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPLAYGROUND_API UPGAbilityComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UPGAbilityComponent();
    
    // 능력 관리
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool GiveAbility(TSubclassOf<class UPGAbility> AbilityClass);
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool RemoveAbility(FName AbilityID);
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    class UPGAbility* GetAbility(FName AbilityID) const;
    
    // 능력 활성화
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool ActivateAbility(FName AbilityID);
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool CanActivateAbility(FName AbilityID) const;
    
    // 리소스 관리 (마나, 스태미나 등)
    UFUNCTION(BlueprintCallable, Category = "Ability|Resources")
    float GetResource(EPGResourceType ResourceType) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability|Resources")
    float GetMaxResource(EPGResourceType ResourceType) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability|Resources")
    bool ConsumeResource(EPGResourceType ResourceType, float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Ability|Resources")
    void SetResource(EPGResourceType ResourceType, float NewValue);
    
    UFUNCTION(BlueprintCallable, Category = "Ability|Resources")
    void SetMaxResource(EPGResourceType ResourceType, float NewValue);
    
    // 이벤트
    UPROPERTY(BlueprintAssignable, Category = "Ability")
    FOnAbilityActivated OnAbilityActivated;
    
    UPROPERTY(BlueprintAssignable, Category = "Ability")
    FOnAbilityEnded OnAbilityEnded;
    
    UPROPERTY(BlueprintAssignable, Category = "Ability")
    FOnCooldownStarted OnCooldownStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Ability")
    FOnCooldownEnded OnCooldownEnded;
    
    UPROPERTY(BlueprintAssignable, Category = "Ability|Resources")
    FOnResourceChanged OnResourceChanged;
    
protected:
    // 보유 능력
    UPROPERTY()
    TMap<FName, class UPGAbility*> Abilities;
    
    // 리소스 값 (마나, 스태미나 등)
    //UPROPERTY(ReplicatedUsing = OnRep_Resources)
    TMap<EPGResourceType, float> CurrentResources;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Resources")
    TMap<EPGResourceType, float> MaxResources;
    
    // 리소스 재생성 속도
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability|Resources")
    TMap<EPGResourceType, float> ResourceRegenRates;
    
    // 리소스 복제 콜백
    UFUNCTION()
    void OnRep_Resources();
    
    // 컴포넌트 라이프사이클
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // 리소스 재생성
    void RegenerateResources(float DeltaTime);
    
    // 능력 인스턴스 생성
    class UPGAbility* CreateAbilityInstance(TSubclassOf<class UPGAbility> AbilityClass);
};
