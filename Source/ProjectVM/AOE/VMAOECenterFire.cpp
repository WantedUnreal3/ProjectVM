// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOECenterFire.h"

#include "Engine/OverlapResult.h"
#include "Components/SceneComponent.h"

#include "Particles/ParticleSystem.h"

#include "Sound/SoundBase.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AVMAOECenterFire::AVMAOECenterFire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	/*ConstructorHelpers::FObjectFinder<UParticleSystem> AOEEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	AOEEffect = AOEEffectRef.Object;

	ConstructorHelpers::FObjectFinder<USoundBase> AOESoundRef(TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	AOESound = AOESoundRef.Object;*/
}

// Called when the game starts or when spawned
void AVMAOECenterFire::BeginPlay()
{
	Super::BeginPlay();

	// 스폰된 순간 월드의 Player 찾아 이벤트 바인딩
	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(Player);
		if (Hero)
		{
			OnAOESphereOverlapActor.AddDynamic(Hero, &AVMCharacterHeroBase::OnHitDotByAOE);
		}
	}

	TriggerSpawnAOESphere();
}

// Called every frame
void AVMAOECenterFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMAOECenterFire::TriggerSpawnAOESphere()
{
	// 호출되면 이게 실행

	//Bind랑 다할거임.
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AVMAOECenterFire::SpawnAOESphere, OverlapTick, true);
	GetWorld()->GetTimerManager().SetTimer(FinishTimeHandle, this, &AVMAOECenterFire::DestroyWrapper, MaxDurationTime, false);
}

void AVMAOECenterFire::SpawnAOESphere()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AVMAOEBase::SpawnAOESphere] World is nullptr"));
		return;
	}

	FVector Center = GetActorLocation();
	UE_LOG(LogTemp, Log, TEXT("Center is (%f, %f, %f)"), Center.X, Center.Y, Center.Z);

#pragma region Debug용 코드
	if (bDrawDebugSphere == true)
	{
		FColor Color = FColor::Green;
		DrawDebugSphere(World, Center, Radius, 16, Color, false, 10.0f, 0, 1.0f);
	}
#pragma endregion 

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

	UParticleSystem* ParticleSystem = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_Fire_Big.P_Fire_Big'"));
	if (ParticleSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("불 확인"));
		return;
	}

	FVector Scale(5, 5, 5);
	UGameplayStatics::SpawnEmitterAttached(ParticleSystem, RootComponent, TEXT("NoName"), FVector::Zero(), GetActorRotation(), Scale, EAttachLocation::KeepRelativeOffset);



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
				BroadCastFireOverlapActor(OverlappedActor);
			}
		}
	}
}

void AVMAOECenterFire::DestroyWrapper()
{
	Destroy();
}

void AVMAOECenterFire::BroadCastFireOverlapActor(AActor* Actor)
{
	UE_LOG(LogTemp, Log, TEXT("BroadCastFireOverlapActor"));
	OnAOESphereOverlapActor.Broadcast(Actor);
	// Actor를 굳이 보내야 함?

	// 1. 윤성님 부분에 화염 스택을 쌓는다.
	// 2. 윤성님 부분에 타이머를 돌린다.
	// 3. 데미지는 스택이 쌓인 만큼 얻는다.
	// 4. 시간 초가 다 꺼지기 전에 불에 다시 닿는다면, 시간을 다시 10초 센다.
	// 5. 타이머가 끝나면 스택을 0으로 초기화한다.
}

