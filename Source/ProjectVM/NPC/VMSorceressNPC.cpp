#include "VMSorceressNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

AVMSorceressNPC::AVMSorceressNPC()
{
	NPCType = ENPCType::Anna;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 120.0f);
	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("/Game/NPCContent/Sorceress/Sorceress.Sorceress"));
	if (NPCMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(NPCMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -120.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
	// ABP 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPCAnimBP(TEXT("/Game/Project/NPC/ABP_Sorceress.ABP_Sorceress_C"));
	if (NPCAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPCAnimBP.Class);
	}

	InteractKey->SetWorldLocation(FVector(0.0f, 0.0f, 100.0f));
	InteractKeyBoxComponent->SetBoxExtent(FVector(120.0f, 120.0f, 100.0f));
}
