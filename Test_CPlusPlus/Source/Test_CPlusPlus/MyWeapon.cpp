// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SW(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_BatAxe/SK_Blade_BatAxe.SK_Blade_BatAxe'"));
	
	if (SW.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("WeaponFindSuccess"));
		Weapon->SetSkeletalMesh(SW.Object);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("WeaponFindFailed"));
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}
