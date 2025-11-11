// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/EnergyBolt.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/HeroStat.h"
#include "Hero/Alpha/VMEnergyBoltProjectile.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

#include "CollisionQueryParams.h"
#include "Interface/VMStatChangeable.h"
#include "Kismet/GameplayStatics.h"

UEnergyBolt::UEnergyBolt(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("EnergyBolt");
	SkillDesc = TEXT("에너지 볼트를 발사합니다.");
	ManaCost = 10;
	Cooldown = 5;
	RemainingCooldown = 0;
}

void UEnergyBolt::ActivateSkill(AVMCharacterHeroBase* Owner, FHeroStat& CurStat)
{
	Super::ActivateSkill(Owner, CurStat);

	UE_LOG(LogTemp, Log, TEXT("Skill : EnergyBolt !"));

	TArray<FOverlapResult> Targets;
	FVector Center = Owner->GetActorLocation();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(1000.0f);

	bool HitDetected = Owner->GetWorld()->OverlapMultiByChannel(Targets, Center, FQuat::Identity, ECC_GameTraceChannel3, CollisionShape);
	
	FColor DrawDebugColor = FColor::Green;
	if (HitDetected) DrawDebugColor = FColor::Red;

	DrawDebugSphere(Owner->GetWorld(), Center, 1000.0f, 32, DrawDebugColor, false, 2.f);

	UE_LOG(LogTemp, Log, TEXT("타겟을 %d 개 찾았습니다."), Targets.Num());

	for (const FOverlapResult& Target : Targets)
	{
		UE_LOG(LogTemp, Log, TEXT("발견한 타겟 : %s"), *Target.GetActor()->GetName());
		
		IVMStatChangeable* StatChangeable = Cast<IVMStatChangeable>(Target.GetActor());

		if (StatChangeable)
		{
			UE_LOG(LogTemp, Log, TEXT("%s 충돌 !"), *Target.GetActor()->GetName());
			
			StatChangeable->HealthPointChange(10.f, UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		}
		
		//AVMEnergyBoltProjectile* Projectile = Owner->GetWorld()->SpawnActor<AVMEnergyBoltProjectile>(AVMEnergyBoltProjectile::StaticClass(), Owner->GetActorLocation(), FRotator::ZeroRotator);
		//Projectile->BindTarget(Target.GetActor());
	}
}