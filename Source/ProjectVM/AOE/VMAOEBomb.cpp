// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEBomb.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/OverlapResult.h"

#include "Kismet/GameplayStatics.h"

#include "TimerManager.h"

#include "Particles/ParticleSystem.h"           // UParticleSystem
#include "Particles/ParticleSystemComponent.h"  // UParticleSystemComponent
#include "Sound/SoundBase.h"                     // USoundBase

#include "Macro/VMPhysics.h"
#include "GameFramework/Character.h"

// Sets default values
AVMAOEBomb::AVMAOEBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //// SphereCollision
    //SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    //RootComponent = SphereComp;
    //SphereComp->SetSphereRadius(30.f);
    //SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
    //SphereComp->SetCollisionResponseToAllChannels(ECR_Block);
    //SphereComp->OnComponentHit.AddDynamic(this, &AVMAOEBomb::OnHit);

    // Mesh
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    MeshComp->OnComponentHit.AddDynamic(this, &AVMAOEBomb::OnHit);
    ConstructorHelpers::FObjectFinder<UStaticMesh>MeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (MeshRef.Object)
    {
        MeshComp->SetStaticMesh(MeshRef.Object);
    }

    //MeshComp->SetSimulatePhysics(true);         // Physics Simulation 켬
    MeshComp->SetEnableGravity(true);          // 중력 적용
    MeshComp->SetNotifyRigidBodyCollision(true); // OnHit 이벤트 받기
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionObjectType(ECC_WorldDynamic); // 오브젝트 타입
    MeshComp->SetCollisionResponseToAllChannels(ECR_Block); // 모든 채널 Block
    
    
    //MeshComp->SetMassOverrideInKg(NAME_None, 5.f); // 무게 설정
}

// Called when the game starts or when spawned
void AVMAOEBomb::BeginPlay()
{
	Super::BeginPlay();
	
    ExplosionTime = FMath::RandRange(2.0, 5.0);
}

// Called every frame
void AVMAOEBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMAOEBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (flag == true)
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("OnHit"));
    UE_LOG(LogTemp, Log, TEXT("%f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
    flag = true;

    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AVMAOEBomb::Explode, ExplosionTime, false);

    
    // 충돌 이후 더 이상 Physics 충돌 적용 안 하게 선택 가능
    //SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //MeshComp->SetSimulatePhysics(false);
}

void AVMAOEBomb::Explode()
{
    UE_LOG(LogTemp, Display, TEXT("Explode 들어옴"));
    FVector ExplosionLocation = GetActorLocation();
    float Radius = 300.f;

    // 1️⃣ 파티클 & 사운드
    if (ExplosionEffect == nullptr)
    {
        ExplosionEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, ExplosionLocation);
    }

    if (ExplosionSound == nullptr)
    {
        ExplosionSound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, ExplosionLocation);
    }

    // 범위 내 Actor 찾기
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

    DrawDebugSphere(
        GetWorld(),
        ExplosionLocation,
        Radius,
        32,                   // 세그먼트 수
        FColor::Green,        // 색상
        false,                // 지속 시간 (false: Tick 한번만)
        5.f,                  // 지속 시간 (초)
        0,                    // 두께
        2.f                   // 선 두께
    );

    bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        ExplosionLocation,
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
                UE_LOG(LogTemp, Warning, TEXT("이거오면 거의 끝"));
                //// 3️⃣ 데미지 주기
                //UGameplayStatics::ApplyDamage(HitActor, 50.f, nullptr, this, UDamageType::StaticClass());
                ACharacter* Char = Cast<ACharacter>(HitActor);
                if (Char)
                {
                    FVector LaunchDir = (HitActor->GetActorLocation() - ExplosionLocation).GetSafeNormal();
                    LaunchDir.Z += 0.5f; // 위로 띄우기
                    Char->LaunchCharacter(LaunchDir * 1000.f, true, true);
                }
                // 4️⃣ Launch impulse 주기
                //UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
                //if (PrimComp && PrimComp->IsSimulatingPhysics())
                //{
                //    UE_LOG(LogTemp, Log, TEXT("너는 뭐세요?"));
                //    FVector LaunchDir = (HitActor->GetActorLocation() - ExplosionLocation).GetSafeNormal();
                //    PrimComp->AddImpulse(LaunchDir * 1000.f); // 세기 조절
                //}
            }
        }
    }
    Destroy();
}

