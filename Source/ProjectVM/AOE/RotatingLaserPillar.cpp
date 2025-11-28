// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/RotatingLaserPillar.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Hero/VMCharacterHeroBase.h"

#include "AI/VMEnemyBase.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Particles/ParticleSystemComponent.h"


ARotatingLaserPillar::ARotatingLaserPillar()
{
    PrimaryActorTick.bCanEverTick = true;
    
    PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));

    RootComponent = PillarMesh;

    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ParagonGideon/FX/Meshes/Heroes/Gideon/SM_Black_Hole_Main.SM_Black_Hole_Main'"));
    if (MeshRef.Object)
    {
        PillarMesh->SetStaticMesh(MeshRef.Object);
    }

    PillarMesh->SetCollisionProfileName(TEXT("NoCollision"));

    LaserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
    LaserParticle->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(
        TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_laser01.P_ky_laser01'")
    );

    if (ParticleRef.Object)
    {
        LaserParticle->SetTemplate(ParticleRef.Object);
    }



    LaserParticle->SetAutoActivate(true);

    LaserParticle->SetupAttachment(PillarMesh);
    LaserParticle->SetRelativeLocation(FVector(980, 0, 0));
    LaserParticle->SetRelativeRotation(FRotator(90, 0, 0));

    DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollider"));
    DamageCollider->SetupAttachment(PillarMesh);
    DamageCollider->SetBoxExtent(FVector(500, 32.0f, 32.0f));
    DamageCollider->SetRelativeLocation(FVector(500, 0, 0));
    //DamageCollider->SetRelativeRotation(FRotator(0, 0, 90));
    //DamageCollider->SetCollisionProfileName(TEXT("VMTest"));

    // Todo: 케스케이드
    DamageCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &ARotatingLaserPillar::OnDamageOverlap);
    DamageCollider->OnComponentEndOverlap.AddDynamic(this, &ARotatingLaserPillar::OnDamageEndOverlap);

    RotationSpeed = 360.f;      // 기본 회전 속도
    DamageAmount = 20.f;       // 기본 데미지
    DamageInterval = 0.1f;     // 기본 지속 데미지 간격
}

void ARotatingLaserPillar::BeginPlay()
{
    Super::BeginPlay();

    // 10초 후 제거
    SetLifeSpan(10.f);

    // 지속 데미지 타이머 시작
    GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ARotatingLaserPillar::DealDamage, DamageInterval, true);
}

void ARotatingLaserPillar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 회전
    FRotator NewRotation = FRotator(0.f, RotationSpeed * DeltaTime, 0.f);
    PillarMesh->AddLocalRotation(NewRotation);


    const FTransform& ColTM = DamageCollider->GetComponentTransform();
    FVector Extent = DamageCollider->GetScaledBoxExtent();

    // 선 굵기는 1.0f, 색은 빨강, 지속 시간 0(한 프레임), 깊이버퍼 무시 false
    DrawDebugBox(GetWorld(), ColTM.GetLocation(), Extent, ColTM.GetRotation(), FColor::Red, false, 0.0f, 0, 2.0f);
}

void ARotatingLaserPillar::OnDamageOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("OnDamageOverlap"));
    if (OtherActor && OtherActor != this)
    {
        OverlappingActors.Add(OtherActor);
    }
}

void ARotatingLaserPillar::OnDamageEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{    
    UE_LOG(LogTemp, Warning, TEXT("OnDamageEndOverlap"));
    if (OtherActor)
    {
        OverlappingActors.Remove(OtherActor);
    }
}



void ARotatingLaserPillar::DealDamage()
{
    for (AActor* Actor : OverlappingActors)
    {
        // Todo 25-11-19: 캐릭터로 변경해야함

        AVMCharacterHeroBase* EnemyBase = Cast<AVMCharacterHeroBase>(Actor);
        if (EnemyBase == nullptr)
        {
            continue;
        }

        UE_LOG(LogTemp, Log, TEXT("데미지 들어오니?"));
        EnemyBase->HealthPointChange(10, this);
    }
}