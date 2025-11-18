
#include "VMPortal.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/Character.h"

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

	//0.2초 뒤에 맵 이동
	if (GetWorld() != nullptr)
	{
		GetWorld()->GetTimerManager().SetTimer(
			EffectTimerHandle,
			this,
			&AVMPortal::TeleportPlayerToMap,
			0.2f,
			false
		);
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

void AVMPortal::BeginPlay()
{
	Super::BeginPlay();

}

void AVMPortal::TeleportPlayerToMap()
{
	//포탈로 이동하는 이펙트 라인 숨기기
	if (EffectLineNiagaraSystem == nullptr)
	{
		return;
	}
	EffectLineNiagaraSystem->Deactivate(); // 비활성화

	//맵 이동
	if (!GetWorld()) return;

	//평원맵(LevelStreaming) 찾기
	ULevelStreaming* PlainsLevel = nullptr;
	for (ULevelStreaming* StreamingLevel : GetWorld()->GetStreamingLevels())
	{
		if (StreamingLevel && StreamingLevel->GetWorldAssetPackageName().Contains("MainMap")) // 평원맵 이름
		{
			PlainsLevel = StreamingLevel;
			break;
		}
	}

	//평원맵 언로드
	if (PlainsLevel)
	{
		PlainsLevel->SetShouldBeLoaded(false);
		PlainsLevel->SetShouldBeVisible(false);
	}

	//보스맵(LevelStreamingDynamic) 로드
	bool bSuccess = false;
	LoadedLevel = ULevelStreamingDynamic::LoadLevelInstance(
		GetWorld(),
		TargetLevelPath,      // FString 레벨 경로
		FVector::ZeroVector,  // 위치
		FRotator::ZeroRotator,// 회전
		bSuccess
	);

	if (bSuccess && LoadedLevel)
	{
		LoadedLevel->SetShouldBeLoaded(true);
		LoadedLevel->SetShouldBeVisible(true);
	}
}

