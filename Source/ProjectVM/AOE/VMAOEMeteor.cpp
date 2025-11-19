// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEMeteor.h"
#include "AOE/VMAOEFire.h"

#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

#include "Particles/ParticleSystem.h"

#include "Engine/OverlapResult.h"

#include "Hero/VMCharacterHeroBase.h"

#include "GameFramework/CharacterMovementComponent.h"



AVMAOEMeteor::AVMAOEMeteor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComp->SetupAttachment(RootComponent);
    DecalComp->SetHiddenInGame(true);

    ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalCompMeterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/KTP_Decal/Decal/plant_DID_101125.plant_DID_101125'"));
    if (DecalCompMeterialRef.Object)
    {
        DecalComp->SetDecalMaterial(DecalCompMeterialRef.Object);
    }
}

void AVMAOEMeteor::BeginPlay()
{
    Super::BeginPlay();
    CreateLogic();
    /*GetWorld()->GetTimerManager().SetTimer(DecalTimeHandle, [this]()
        {
            CreateLogic();
        }, 3.0f, false);*/
}

void AVMAOEMeteor::CreateLogic()
{
    DecalComp->DecalSize = FVector(256.f, 256.f, 256.f);
    DecalComp->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
    DecalComp->MarkRenderStateDirty(); // 갱신 강제 굳이?
    DecalComp->SetHiddenInGame(false);


    //DecalMaterial, Size, Location, Rotation, 0
    DecalTimeHandle.Invalidate();

    GetWorld()->GetTimerManager().SetTimer(DecalTimeHandle, [this]()
        {
            UE_LOG(LogTemp, Log, TEXT("Hello"));
            if (DecalComp == nullptr)
            {
                UE_LOG(LogTemp, Log, TEXT("No Nullptr"));
            }
            if (IsValid(DecalComp))
            {
                UE_LOG(LogTemp, Log, TEXT("정상이니까 삭제"));
                DecalComp->DestroyComponent();
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("비정상이니까 삭제 안함."));
            }
            InitAOEPosition();
            SpawnAOE();
        }, 3.0f, false);
}

void AVMAOEMeteor::InitAOEPosition()
{
    // z축으로 Lay를 쏴서 부딪힌 Mesh의 좌표를 얻는다.
    DecalLocation = GetActorLocation();
    FVector StartLocation = DecalLocation + FVector(0, 0, 1000);
    FVector EndLocation = DecalLocation + FVector(0, 0, -1000);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);    // 자기 자신 무시

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params);
    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            Location = Hit.Location;
            UE_LOG(LogTemp, Log, TEXT("맞긴하니?"));
        }
    }
    FColor LineColor = bHit ? FColor::Red : FColor::Green;

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, true, 2.0f, 0, 2.0f);
    UE_LOG(LogTemp, Log, TEXT("끝난거니?"));
}

void AVMAOEMeteor::SpawnAOE()
{
    UE_LOG(LogTemp, Display, TEXT("(%f,%f,%f) (%f,%f,%f)"), Location.X, Location.Y, Location.Z, GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

    USoundBase* MySound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
    if (MySound == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("엥?"));
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(
        this,           // World context (보통 Actor나 UObject)
        MySound,    // USoundBase* 사운드
        GetActorLocation(),  // 재생 위치
        1.0f,           // 볼륨
        1.0f            // 피치
    );

    UParticleSystem* ParticleSystem = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
    if (ParticleSystem == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("운석이 안나오는데?"));
        return;
    }

    UGameplayStatics::SpawnEmitterAttached(ParticleSystem, RootComponent, TEXT("NoName"), FVector::Zero(), GetActorRotation(), EAttachLocation::KeepRelativeOffset);


    // Sphere를 그리고 충돌 처리를 하는게 나을 듯?
    // 충돌 채널 설정
    TArray<FOverlapResult> Overlaps;
    float Radius = 256.0f;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
    bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, Location, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), Sphere);
    if (bHasOverlap)
    {
        for (auto& Result : Overlaps)
        {
            AActor* OverlappedActor = Result.GetActor();
            if (OverlappedActor)
            {
                UE_LOG(LogTemp, Log, TEXT("Overlapped: %s"), *OverlappedActor->GetName());
                FString Debug = FString::Printf(TEXT("Name: %s"), *OverlappedActor->GetName());
                GEngine->AddOnScreenDebugMessage(
                    -1,                 // Key, -1이면 새 메시지
                    5.0f,               // 화면에 표시될 시간(초)
                    FColor::Red,        // 텍스트 색상
                    Debug // 출력할 문자열
                );
                //Alpha의 CharacterMovement를 건들여보자.
                AVMCharacterHeroBase* HeroPawn = Cast<AVMCharacterHeroBase>(OverlappedActor);
                if (HeroPawn == nullptr)
                {
                    continue;
                }
                
                BroadcastOverlapActor(HeroPawn, 40);
            }
        }
    }
#pragma region Debug용 코드
    DrawDebugSphere(GetWorld(), Location, Radius, 16, FColor::Green, false, 10.0f, 0, 1.0f);
#pragma endregion 

    AVMAOEFire* FireSpawnActor = GetWorld()->SpawnActor<AVMAOEFire>(AVMAOEFire::StaticClass(), GetActorTransform());
    if (FireSpawnActor == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("운석 충돌 -> 불 소환 실패"));
    }
}

void AVMAOEMeteor::BroadcastOverlapActor(AActor* Actor, float InDamage)
{
    UE_LOG(LogTemp, Log, TEXT("AVMAOEMeteor::BroadcastOverlapActor"));
    OnAOEMeteorOverlapActor.Broadcast(Actor, InDamage);
}
