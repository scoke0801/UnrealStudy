// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Prop/CWItemBox.h"
#include "BasicTest/Character/Interface/CWCharacterItemIntercae.h"
#include "BasicTest/Physics/CWCollision.h"
#include "BasicTest/Data/CWItemData.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Engine/AssetManager.h"

// Sets default values
ACWItemBox::ACWItemBox()
{
	_triggerBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = _triggerBoxComp;
	_mesh->SetupAttachment(_triggerBoxComp);
	_effect->SetupAttachment(_triggerBoxComp);

	_triggerBoxComp->SetCollisionProfileName(CPROFILE_CWTRIGGER);
	_triggerBoxComp->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Game/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		_mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	_mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	_mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Game/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (EffectRef.Object)
	{
		_effect->SetTemplate(EffectRef.Object);
		_effect->bAutoActivate = false;
	}
}

void ACWItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> AssetIds;
	Manager.GetPrimaryAssetIdList(TEXT("CWItemData"), AssetIds);
	ensure(0 < AssetIds.Num());

	int32 RandomIndex = FMath::RandRange(0, AssetIds.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(AssetIds[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	_item = Cast<UCWItemData>(AssetPtr.Get());
	ensure(_item);

	_triggerBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACWItemBox::OnOverlapBegin);

}

void ACWItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == _item)
	{
		Destroy();
		return;
	}

	if (ICWCharacterItemIntercae* OverlappingPawn = Cast<ICWCharacterItemIntercae>(OtherActor))
	{
		OverlappingPawn->TakeItem(_item);
	}

	_effect->Activate(true);

	_mesh->SetHiddenInGame(true);

	SetActorEnableCollision(false);

	_effect->OnSystemFinished.AddDynamic(this, &ACWItemBox::OnEffectFinished);
}

void ACWItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}
