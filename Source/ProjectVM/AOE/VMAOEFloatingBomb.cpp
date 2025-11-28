// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEFloatingBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Engine/OverlapResult.h"

AVMAOEFloatingBomb::AVMAOEFloatingBomb()
{
    PrimaryActorTick.bCanEverTick = true;


    // CollisionSphere를 루트로 설정
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(100.f); // 원하는 범위
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AVMAOEFloatingBomb::OnOverlapBegin);

    // Mesh를 CollisionSphere 아래에 붙임
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(CollisionSphere);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ParagonMinions/FX/Meshes/Icons/SM_Visbility_Eye.SM_Visbility_Eye'"));
    if (MeshRef.Object)
    {
        Mesh->SetStaticMesh(MeshRef.Object);
    }
}

void AVMAOEFloatingBomb::BeginPlay()
{
    Super::BeginPlay();

    PlayerActor = Cast<AVMCharacterHeroBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    FVector NewLocation = GetActorLocation();
    SetActorLocation(NewLocation);
}

void AVMAOEFloatingBomb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerActor == nullptr)
    {
        return;
    }

    
    // 플레이어 바라보기
    FVector Direction = PlayerActor->GetActorLocation() - GetActorLocation();
    Direction.Z = 0;
    FRotator TargetRotation = Direction.Rotation();
    FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
    SetActorRotation(NewRotation);
    
}

// Overlap 이벤트
void AVMAOEFloatingBomb::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        // 플레이어만 폭발 대상으로 하고 싶으면 체크
        if (OtherActor == PlayerActor)
        {
            Explode();
        }
    }
}

void AVMAOEFloatingBomb::Explode()
{
    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
    }

    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);

    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        ExplosionRadius,
        32,                   // 세그먼트 수
        FColor::Green,        // 색상
        false,                // 지속 시간 (false: Tick 한번만)
        5.f,                  // 지속 시간 (초)
        0,                    // 두께
        2.f                   // 선 두께
    );

    bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        GetActorLocation(),
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Pawn), // 캐릭터/적용할 채널
        Sphere
    );
    UE_LOG(LogTemp, Log, TEXT("flag:%d Count:%d"), bHasOverlap, Overlaps.Num());

    if (bHasOverlap)
    {
        for (auto& Result : Overlaps)
        {
            AActor* HitActor = Result.GetActor();
            if (HitActor)
            {
                AVMCharacterHeroBase* Char = Cast<AVMCharacterHeroBase>(HitActor);
                if (Char)
                {
                    FVector LaunchDir = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
                    LaunchDir.Z += 0.5f; // 위로 띄우기
                    Char->LaunchCharacter(LaunchDir * 1000.f, true, true);
                    Char->HealthPointChange(20, this);
                }
            }
        }
    }

    Destroy();
}