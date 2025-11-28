// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEMine.h"

#include "Components/CapsuleComponent.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Engine/OverlapResult.h"

// Sets default values
AVMAOEMine::AVMAOEMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Cylinder (Capsule) collision as root
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CylinderCollision"));
	RootComponent = Collision;
	Collision->InitCapsuleSize(50.f, 60.f); // 반지름, 높이(원통 형태로 조정)
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	// Mesh
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MineMesh->SetRelativeScale3D(FVector(1, 1, 0.2));
	MineMesh->SetRelativeLocation(FVector(0, 0, -10));
	MineMesh->SetupAttachment(RootComponent);
	MineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (MeshRef.Object)
	{
		MineMesh->SetStaticMesh(MeshRef.Object);
	}

	// 초기값
	OverlappingCharacter = nullptr;
	bIsDisarmed = false;
}

// Called when the game starts or when spawned
void AVMAOEMine::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AVMAOEMine::OnBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AVMAOEMine::OnEndOverlap);
}

// Called every frame
void AVMAOEMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMAOEMine::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이미 해제되었거나 이미 누군가 저장된 상태면 무시 (원하면 다중 지원으로 바꿀 수 있음)
	if (bIsDisarmed || OverlappingCharacter != nullptr)
	{
		return;
	}

	if (AVMCharacterHeroBase* Character = Cast<AVMCharacterHeroBase>(OtherActor))
	{
		OverlappingCharacter = Character;

		// 5초 후 Disarm (플레이어가 5초 동안 밟고 있으면 해제)
		GetWorldTimerManager().SetTimer(DisarmTimerHandle, this, &AVMAOEMine::Disarm, 5.0f, false);
	}
}

void AVMAOEMine::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 만약 저장된 캐릭터가 나간 경우
	if (OtherActor == OverlappingCharacter)
	{
		// 이미 해제되었다면 아무 처리 안 함
		if (bIsDisarmed)
		{
			// cleanup
			OverlappingCharacter = nullptr;
			if (GetWorldTimerManager().IsTimerActive(DisarmTimerHandle))
			{
				GetWorldTimerManager().ClearTimer(DisarmTimerHandle);
			}
			return;
		}

		// 아직 해제되지 않았다면 — 떠나기 때문에 폭발
		// 타이머 있으면 클리어
		if (GetWorldTimerManager().IsTimerActive(DisarmTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(DisarmTimerHandle);
		}

		Detonate();
	}
}

void AVMAOEMine::Detonate()
{
	UE_LOG(LogTemp, Log, TEXT("폭발"));

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
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
				UE_LOG(LogTemp, Warning, TEXT("이거오면 거의 끝"));
				//// 3️⃣ 데미지 주기
				//UGameplayStatics::ApplyDamage(HitActor, 50.f, nullptr, this, UDamageType::StaticClass());
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

void AVMAOEMine::Disarm()
{
	// 사라지는 이펙트 넣으면 좋아보임.
	UE_LOG(LogTemp, Log, TEXT("해제"));

	// 마인이 해제되었음을 표시
	bIsDisarmed = true;

	// 타이머 클리어(안전)
	if (GetWorldTimerManager().IsTimerActive(DisarmTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DisarmTimerHandle);
	}

	// 이후 원하는 동작: 시각적으로 바꾸거나(예: 색상, 메시 교체), 충돌 비활성화 등
	// 예: 충돌을 비활성화해서 더 이상 트리거하지 않게 할 수도 있음
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Destroy();
}
