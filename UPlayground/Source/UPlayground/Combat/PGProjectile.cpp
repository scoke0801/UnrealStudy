#include "PGProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PGDamageType.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"

APGProjectile::APGProjectile()
{
    // 이 액터가 네트워크 복제를 사용할 수 있도록 설정
    bReplicates = true;
    
    // 물리 충돌 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionProfileName("Projectile");
    CollisionComponent->OnComponentHit.AddDynamic(this, &APGProjectile::OnProjectileHit);
    
    // 콜리전 컴포넌트를 루트로 설정
    RootComponent = CollisionComponent;
    
    // 프로젝타일 무브먼트 컴포넌트 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->SetUpdatedComponent(CollisionComponent);
    ProjectileMovement->InitialSpeed = 2000.0f;
    ProjectileMovement->MaxSpeed = 2000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;  // 중력 없음
    
    // 파티클 컴포넌트 설정
    ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
    ParticleComponent->SetupAttachment(RootComponent);
    
    // 오디오 컴포넌트 설정
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);
    AudioComponent->bAutoActivate = true;
    
    // 기본 수명 설정
    LifeSpan = 5.0f;
    bDestroyOnHit = true;
    
    // 기본 데미지 설정
    Damage = 20.0f;
    
    // 틱 활성화
    PrimaryActorTick.bCanEverTick = true;
}

void APGProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    // 수명 설정
    SetLifeSpan(LifeSpan);
}

void APGProjectile::InitializeProjectile(AActor* ProjectileOwner, AActor* InTargetActor, float InDamage, TSubclassOf<UPGDamageType> InDamageType)
{
    if (ProjectileOwner)
    {
        InstigatorActor = ProjectileOwner;
        SetInstigator(Cast<APawn>(ProjectileOwner));
        SetOwner(ProjectileOwner);
    }
    
    TargetActor = InTargetActor;
    Damage = InDamage;
    DamageType = InDamageType;
    
    // 타겟이 있으면 추적 설정
    if (TargetActor && ProjectileMovement)
    {
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
        ProjectileMovement->HomingAccelerationMagnitude = 1000.0f;
    }
}

void APGProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    
    // 자신 또는 소유자에게 충돌하면 무시
    if (Other == this || Other == GetOwner())
    {
        return;
    }
    
    // 히트 처리
    OnProjectileHit(MyComp, Other, OtherComp, NormalImpulse, Hit);
}

void APGProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 자신 또는 소유자에게 충돌하면 무시
    if (OtherActor == this || OtherActor == GetOwner())
    {
        return;
    }
    
    // 데미지 적용
    if (GetLocalRole() == ROLE_Authority && OtherActor)
    {
        UClass* DamageTypeClass = DamageType.Get() ? DamageType.Get() : UDamageType::StaticClass();
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageTypeClass);
    }
    
    // 히트 이펙트 재생
    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, Hit.Normal.Rotation());
    }
    
    if (NiagaraImpactEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraImpactEffect, Hit.Location, Hit.Normal.Rotation());
    }
    
    // 히트 사운드 재생
    if (ImpactSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.Location);
    }
    
    // 충돌 시 발사체 파괴 설정
    if (bDestroyOnHit)
    {
        Destroy();
    }
}
