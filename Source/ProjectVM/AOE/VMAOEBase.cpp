// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/OverlapResult.h"

#include "Hero/VMCharacterHeroBase.h"

#pragma region 특수 멤버 함수

AVMAOEBase::AVMAOEBase()
{
	UE_LOG(LogTemp, Log, TEXT("AVMAOEBase 생성자"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = SceneRootComp;

	Radius = 300.0f;

	bDrawDebugSphere = true;
}

#pragma endregion 

#pragma region 엔진 제공 함수

void AVMAOEBase::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("AVMAOEBase BeginPlay"));
	Super::BeginPlay();

	//TriggerSpawnAOESphere();
}

// Called every frame
void AVMAOEBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma endregion 

void AVMAOEBase::SpawnAOESphere()
{
	UE_LOG(LogTemp, Log, TEXT("SpawnAOESphere"));
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AVMAOEBase::SpawnAOESphere] World is nullptr"));
		return;
	}

	FVector Center = GetActorLocation();
	UE_LOG(LogTemp, Log, TEXT("Center is (%f, %f, %f)"), Center.X, Center.Y, Center.Z);

//#pragma region Debug용 코드
//	if (bDrawDebugSphere == true)
//	{
//		Color = FColor::Green;
//		DrawDebugSphere(World, Center, Radius, 16, Color, false, 10.0f, 0, 1.0f);
//	}
//#pragma endregion 

	// 충돌 채널 설정
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	bool bHasOverlap = World->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), Sphere);
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
				FOnAOEOverlapActorWithCenter(OverlappedActor, Center);
			}
		}
	}
}

void AVMAOEBase::TriggerSpawnAOESphere()
{
	SpawnAOESphere();
}

// 델리게이트
void AVMAOEBase::FOnAOEOverlapActor(AActor* Target)
{
	UE_LOG(LogTemp, Log, TEXT("FOnAOEOverlapActor를 실행합니다.Target:%s"), *Target->GetName());
	OnAOEOverlapActor.Broadcast(Target);


	// 캐릭터인지 확인
	AVMCharacterHeroBase* HitPawn = Cast<AVMCharacterHeroBase>(Target);
	if (!HitPawn) return;

	// 폭발 중심 -> Target 방향
	FVector Direction = HitPawn->GetActorLocation() - GetActorLocation();
	Direction.Z = 0; // 수평만 적용
	Direction.Normalize();

	// 넉백 세기
	float LaunchStrength = 1500.f;
	FVector LaunchVelocity = Direction * LaunchStrength;
	LaunchVelocity.Z = 500.f; // 수직력

	// 넉백 적용
	HitPawn->LaunchCharacter(LaunchVelocity, true, true);
}

void AVMAOEBase::FOnAOEOverlapActorWithCenter(AActor* Target, FVector ExplosionCenter)
{
	UE_LOG(LogTemp, Log, TEXT("FOnAOEOverlapActor를 실행합니다.Target:%s"), *Target->GetName());
	OnAOEExplosionOverlapActor.Broadcast(Target, ExplosionCenter);
}
