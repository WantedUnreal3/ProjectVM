// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VMSpawnEnemyAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MeshComponent.h"

UVMSpawnEnemyAnimInstance::UVMSpawnEnemyAnimInstance()
{
}

void UVMSpawnEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningActor = Cast<ACharacter>(GetOwningActor());
	if (OwningActor == nullptr)
	{
		return;
	}

	CharacterMovementComp = OwningActor->GetCharacterMovement();
}

void UVMSpawnEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovementComp == nullptr)
	{
		return;
	}
	
	MoveSpeed = CharacterMovementComp->Velocity.Size2D();


	FVector MoveDir = CharacterMovementComp->Velocity.GetSafeNormal();

	// 캐릭터의 전방 벡터 가져오기
	FVector Forward = OwningActor->GetActorForwardVector();

	// 전방과 이동방향의 각도 계산
	float ForwardDot = FVector::DotProduct(Forward, MoveDir);
	float RightDot = FVector::DotProduct(OwningActor->GetActorRightVector(), MoveDir);

	// 라디안 → 도(degree)
	float Angle = FMath::RadiansToDegrees(FMath::Acos(ForwardDot));

	// 좌우 방향 보정 (오른쪽이면 +, 왼쪽이면 –)
	if (RightDot < 0)
		Angle *= -1.f;

	MoveDirection = Angle; // -180 ~ +180
}
