// Fill out your copyright notice in the Description page of Project Settings.


#include "VMWolfNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

AVMWolfNPC::AVMWolfNPC()
{
	NPCType = ENPCType::Bob;

	GetCapsuleComponent()->InitCapsuleSize(80.f, 120.0f);
	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("/Game/NPCContent/Wolf/Wolf.Wolf"));
	if (NPCMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(NPCMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(-15.0f, 0.0f, -120.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	// ABP 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPCAnimBP(TEXT("/Game/Project/NPC/ABP_Wolf.ABP_Wolf_C"));
	if (NPCAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPCAnimBP.Class);
	}

	InteractKey->SetWorldLocation(FVector(0.0f, 0.0f, 100.0f));
	InteractKeyBoxComponent->SetBoxExtent(FVector(120.0f, 120.0f, 100.0f));
}
