// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/VMHomingProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/PrimitiveComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectVMCharacter.h"
#include "AI/VMEnemyBase.h"

#include "Interface/VMStatChangeable.h"
#include "Hero/VMCharacterHeroBase.h"

// Sets default values
AVMHomingProjectile::AVMHomingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Box 콜라이더 생성 및 초기화
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collider->SetBoxExtent(FVector(50, 25, 25));
	Collider->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// 루트 컴포넌트 지정
	RootComponent = Collider;

	// Mesh 생성 및 BoxCollision에 붙임.
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Mesh->SetupAttachment(Collider);

	// Arrow 생성 및 BoxCollision에 붙임.
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Collider);
	Arrow->SetRelativeLocation(FVector(50, 0, 0));

	// Arrow 생성 및 BoxCollision에 붙임.
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(Collider);
	Particle->SetRelativeLocation(FVector(30, 0, 0));

	// ProjectileMovement 생성. Actor 컴포넌트이므로 붙일 필요 없음.
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

#pragma region 에셋로드
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_Projectile.P_Gideon_Burden_Projectile'"));
	ensureAlways(ParticleRef.Object);
	if (ParticleRef.Object)
	{
		Particle->SetTemplate(ParticleRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitWorld.P_Gideon_Burden_HitWorld'"));
	ensureAlways(HitParticleRef.Object);
	if (HitParticleRef.Object)
	{
		HitParticle = HitParticleRef.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> HitSoundRef(TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	ensureAlways(HitSoundRef.Object);
	if (HitSoundRef.Object)
	{
		HitSound = HitSoundRef.Object;
	}
#pragma endregion

	MaxSpeed = InitialSpeed = 500.0f;

	// 투사체 속도 설정.
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;

	// 중력 설정.
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AVMHomingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (Collider)
	{
		//UE_LOG(LogTemp, Log, TEXT("BeginPlay Name:%s"), *GetOwner()->GetName());
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 2️⃣ 다음 프레임(한 틱 뒤)에 충돌 다시 켜고 Ignore 설정
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AVMHomingProjectile::EnableCollision);
		Collider->IgnoreActorWhenMoving(GetOwner(), true);
		Collider->OnComponentHit.AddDynamic(this, &AVMHomingProjectile::HitAndDestroy);
		if (Target)
		{
			UE_LOG(LogTemp, Log, TEXT("Target:%s"), *Target->GetName());
			ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
		}
	}
}

void AVMHomingProjectile::EnableCollision()
{

	if (GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("EnableCollision Ignore:%s"), *GetOwner()->GetName());
		Collider->IgnoreActorWhenMoving(GetOwner(), true);
	}

	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called every frame
void AVMHomingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMHomingProjectile::SetTarget(AActor* NewTarget)
{

	Target = NewTarget;

	UE_LOG(LogTemp, Log, TEXT("TargetName:%s"), *Target->GetName());

	if (ProjectileMovement && Target)
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
	}
}

void AVMHomingProjectile::HitAndDestroy(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("AVMStraightProjectile::HitAndDestroy"));

	// 피격 시 이펙트 On
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), false);

	// 피격 시 소리 재생 On
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.ImpactPoint);


	AVMCharacterHeroBase* Stat = Cast<AVMCharacterHeroBase>(OtherActor);
	if (Stat == nullptr)
	{
		Destroy();
		return;
	}

	// TODO: this가 맞을 지 쏜 애가 맞을지?
	Stat->HealthPointChange(10, this);
	// 객체 파괴
	Destroy();
}
