
#include "VMPortal.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
AVMPortal::AVMPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->InitCapsuleSize(70.0f, 150.0f);
	RootComponent = Capsule;

	// 나이아가라 컴포넌트 생성 및 Capsule에 붙이기
	PortalNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PortalNiagara"));
	PortalNiagaraSystem->SetupAttachment(RootComponent);

	// 나이아가라 시스템 에셋 할당
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> PortalNiagaraRef(TEXT("/Game/PortalSystem/FX/NS_Portal_02.NS_Portal_02"));
	if (PortalNiagaraRef.Succeeded())
	{
		PortalNiagaraSystem->SetAsset(PortalNiagaraRef.Object);
		PortalNiagaraSystem->bAutoActivate = true; // 생성과 동시에 자동 재생
	}

	EffectLineNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectLineNiagara"));
	EffectLineNiagaraSystem->SetupAttachment(PortalNiagaraSystem);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EffectLineNiagaraRef(TEXT("/Game/PortalSystem/FX/NS_SpeedDashLines.NS_SpeedDashLines"));
	if (EffectLineNiagaraRef.Succeeded())
	{
		EffectLineNiagaraSystem->SetAsset(EffectLineNiagaraRef.Object);
		EffectLineNiagaraSystem->bAutoActivate = false; // 생성과 동시에 꺼짐
	}

	//상호작용 컴포넌트 추가
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetupAttachment(RootComponent);
}

void AVMPortal::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Portal 진입"));

	//이펙트 라인 활성화
	if (EffectLineNiagaraSystem)
	{
		EffectLineNiagaraSystem->Activate();
	}

	//플레이어 점프 이동
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter) return;

	// 점프 높이
	float JumpZ = 400.f; // Z축 점프 속도

	// 플레이어가 바라보는 방향으로 이동 거리 조절
	float ForwardSpeed = 1000.f; // 전방 이동 속도, 필요하면 조절
	FVector ForwardVector = PlayerCharacter->GetActorForwardVector(); // 캐릭터 앞 방향

	// 최종 점프 속도
	FVector JumpVelocity = ForwardVector * ForwardSpeed + FVector(0.f, 0.f, JumpZ);

	// 캐릭터 점프 실행
	PlayerCharacter->LaunchCharacter(JumpVelocity, true, true);
}

void AVMPortal::TeleportPlayerToMap()
{
	//자식에서 사용

	// 플레이어 가져오기
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player == nullptr)
	{
		return;
	}

	FVector NewLocation = MapTeleportPos + PlayerTeleportPos; // 보스맵 중앙 위쪽
	Player->SetActorLocation(NewLocation);
	//Player->SetActorRotation(PlayerTeleportRot);
	UE_LOG(LogTemp, Log, TEXT("Player Teleport"));

	//이동 위치에 포탈 생성 후 시간 지나면 사라지게 하기
	TSubclassOf<AVMPortal> ActorToSpawn = AVMPortal::StaticClass();
	AVMPortal* SpawnedPortal = GetWorld()->SpawnActor<AVMPortal>(ActorToSpawn, NewLocation, Player->GetActorRotation());

	if (SpawnedPortal == nullptr)
	{
		return;
	}

	//2초 후에 나이아가라 비활성화
	FTimerHandle FadeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, [SpawnedPortal]()
		{
			if (SpawnedPortal->GetPortalNiagaraSystem() == nullptr)
			{
				return;
			}
			// Deactivate로 비활성화. 나이아가라가 자체적으로 Fade 하면서 사라짐.
			SpawnedPortal->GetPortalNiagaraSystem()->Deactivate();
		}, 2.f, false);

	//안전하게 텀을 주고 5초 후에 포탈 삭제
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [SpawnedPortal]()
		{
			UE_LOG(LogTemp, Log, TEXT("Portal destroy"));
			SpawnedPortal->Destroy();
		}, 5.f, false);
}

void AVMPortal::BeginPlay()
{
	Super::BeginPlay();

}


