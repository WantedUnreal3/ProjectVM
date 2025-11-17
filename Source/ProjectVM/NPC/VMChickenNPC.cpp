#include "NPC/VMChickenNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"

AVMChickenNPC::AVMChickenNPC()
{
	NPCType = ENPCType::Chicken;

	GetCapsuleComponent()->InitCapsuleSize(80.f, 120.0f);
	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("/Game/NPCContent/Chicken/Chicken.Chicken"));
	if (NPCMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(NPCMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(-15.0f, 0.0f, -120.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
	}
	// ABP 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPCAnimBP(TEXT("/Game/Project/NPC/ABP_Chicken.ABP_Chicken_C"));
	if (NPCAnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPCAnimBP.Class);
	}

	InteractKey->SetWorldLocation(FVector(0.0f, 0.0f, 100.0f));
	InteractKeyBoxComponent->SetBoxExtent(FVector(120.0f, 120.0f, 100.0f));
}
