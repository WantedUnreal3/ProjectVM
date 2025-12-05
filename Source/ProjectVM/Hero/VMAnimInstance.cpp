// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMAnimInstance.h"

#include "Hero/VMCharacterHeroBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UVMAnimInstance::UVMAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UVMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Hero = Cast<AVMCharacterHeroBase>(GetOwningActor());
	if (Hero == nullptr) return;
	
	Movement = Hero->GetCharacterMovement();
}

void UVMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement == nullptr) return;
	
	Velocity = Movement->Velocity;

	float ForwardSpeed = FVector::DotProduct(Hero->GetActorForwardVector(), Velocity);
	float RightSpeed   = FVector::DotProduct(Hero->GetActorRightVector(), Velocity);
	LocalVelocity = FVector(ForwardSpeed, RightSpeed, 0.f);
	GroundSpeed = Velocity.Size2D();

	bIsIdle = GroundSpeed < MovingThreshold;
	bIsFalling = Movement->IsFalling();
	bIsJumping = bIsFalling && (Velocity.Z > JumpingThreshold);

	HeroState = Hero->GetHeroState();
}

void UVMAnimInstance::AnimNotify_SkillMotionStart()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify : SkillMotionStart"));
	OnSkillMotionStart.Broadcast();
	OnSkillMotionStart.Clear();
}

void UVMAnimInstance::AnimNotify_SpawnProjectile()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify : SpawnProjectile"));
	OnSpawnProjectile.Broadcast();
	OnSpawnProjectile.Clear();
}

void UVMAnimInstance::AnimNotify_AttackTiming()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify : AttackTiming"));
	OnAttackTiming.Broadcast();
	OnAttackTiming.Clear();
}

void UVMAnimInstance::AnimNotify_SkillMotionEnd()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify : SkillMotionEnd"));
	OnSkillMotionEnd.Broadcast();
	OnSkillMotionEnd.Clear();
}
