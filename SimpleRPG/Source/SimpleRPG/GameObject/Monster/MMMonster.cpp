// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Monster/MMMonster.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMMMonster::AMMMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));

	RootComponent = _body;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("SkeletalMesh'/Game/External/CityofBrass_Enemies/Meshes/Enemy/Grenadier/Grenadier_Mesh.Grenadier_Mesh'"));
	if (MeshRef.Succeeded())
	{
		_body->SetSkeletalMesh( MeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AMMMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMMMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

