// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SW(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_BatAxe/SK_Blade_BatAxe.SK_Blade_BatAxe'"));
	
	if (SW.Succeeded())
	{
		Weapon->SetSkeletalMesh(SW.Object);
	}
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetCollisionProfileName(TEXT("MyCollectable"));

	Trigger->SetupAttachment(Weapon);
	Trigger->SetCollisionProfileName(TEXT("MyCollectable"));
	Trigger->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMyWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnCharacterOveralp);
}

void AMyWeapon::OnCharacterOveralp(UPrimitiveComponent* oerlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		FName WeaponSocket(TEXT("hand_r_socket"));

		AttachToComponent(MyCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
	}

}
