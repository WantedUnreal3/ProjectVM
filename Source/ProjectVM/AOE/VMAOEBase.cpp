// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/OverlapResult.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Macro/VMPhysics.h"

//#include "Utils/VMUtils.h"

#pragma region 특수 멤버 함수

AVMAOEBase::AVMAOEBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneRootComp;
}

#pragma endregion 

#pragma region 엔진 제공 함수

void AVMAOEBase::BeginPlay()
{
	// Parent: AActor
	Super::BeginPlay();
	
	// Sphere Radius 설정
	Radius = 300.0f;
}

void AVMAOEBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma endregion 

void AVMAOEBase::SpawnAOESphere()
{
	// 충돌 채널 파라메터 설정
	TArray<FOverlapResult> Overlaps;
	FVector Center = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	const bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, VM_HERO, Sphere);
	if (bHasOverlap == false)
	{
		return;
	}
	
	// 부딪힌 케이스
	for (auto& Result : Overlaps)
	{
		AActor* OverlappedActor = Result.GetActor();
		if (OverlappedActor == nullptr)
		{
			continue;
		}
		FOnAOEOverlapActorWithCenter(OverlappedActor, Center);
	}
}

void AVMAOEBase::TriggerSpawnAOESphere()
{
	SpawnAOESphere();
}

// 델리게이트
void AVMAOEBase::FOnAOEOverlapActor(AActor* Target)
{
	OnAOEOverlapActor.Broadcast(Target);

	// 히어로의 Base인 AVMCharacterHeroBase 타입 변환
	AVMCharacterHeroBase* HitPawn = Cast<AVMCharacterHeroBase>(Target);
	if (HitPawn == nullptr)
	{
		return;
	}

	// 폭발 중심 -> Target 방향
	FVector Direction = HitPawn->GetActorLocation() - GetActorLocation();
	Direction = Direction.GetSafeNormal2D();	// Direction.Z = 0; 대신 사용.

	// 넉백 세기
	float LaunchStrength = 1500.f;
	FVector LaunchVelocity = Direction * LaunchStrength;
	LaunchVelocity.Z = 500.f; // 수직력

	// 넉백 적용
	HitPawn->LaunchCharacter(LaunchVelocity, true, true);
}

void AVMAOEBase::FOnAOEOverlapActorWithCenter(AActor* Target, const FVector& ExplosionCenter)
{
	UE_LOG(LogTemp, Log, TEXT("FOnAOEOverlapActor를 실행합니다.Target:%s"), *Target->GetName());
	OnAOEExplosionOverlapActor.Broadcast(Target, ExplosionCenter);
}
