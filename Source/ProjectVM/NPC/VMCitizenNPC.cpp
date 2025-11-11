#include "NPC/VMCitizenNPC.h"
#include "Components/CapsuleComponent.h"

AVMCitizenNPC::AVMCitizenNPC()
{
	NPCType = ENPCType::Sue;

	GetCapsuleComponent()->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	// ½ºÄÌ·¹Å» ¸Þ½Ã ¼³Á¤
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("/Game/NPCContent/Chicken/Chicken.Chicken"));
	if (NPCMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(NPCMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		GetMesh()->SetWorldScale3D(FVector(2.3f, 2.3f, 2.3f));
	}
}
