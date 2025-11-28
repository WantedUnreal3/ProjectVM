// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOETargetBomb.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Components/CapsuleComponent.h"

#include "Engine/OverlapResult.h"

// Sets default values
AVMAOETargetBomb::AVMAOETargetBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision
	CylinderCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CylinderCollision"));
	RootComponent = CylinderCollision;
	CylinderCollision->InitCapsuleSize(50.f, 40.f);

	CylinderCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CylinderCollision->SetSimulatePhysics(true);
	CylinderCollision->SetEnableGravity(true);

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AVMAOETargetBomb::BeginPlay()
{
	Super::BeginPlay();
	
	CylinderCollision->OnComponentBeginOverlap.AddDynamic(this, &AVMAOETargetBomb::OnBeginOverlap);
}

// Called every frame
void AVMAOETargetBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMAOETargetBomb::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TriggeringCharacter != nullptr)
	{
		return; // 이미 트리거된 상태면 무시
	}

	if (AVMCharacterHeroBase* Character = Cast<AVMCharacterHeroBase>(OtherActor))
	{
		TriggeringCharacter = Character;

		// 정해진 시간 후 폭발
		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AVMAOETargetBomb::Detonate, ExplosionDelay, false);
	}
}

void AVMAOETargetBomb::Detonate()
{
	// TODO: 이펙트 / 사운드 / 데미지 넣기
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

