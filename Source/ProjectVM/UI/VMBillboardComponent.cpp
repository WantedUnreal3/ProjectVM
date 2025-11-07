#include "UI/VMBillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UVMBillboardComponent::UVMBillboardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UVMBillboardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()) return;

	// 플레이어 컨트롤러 가져오기
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	// 플레이어 카메라 매니저 가져오기
	APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
	if (!CameraManager) return;

	// 카메라 회전 가져오기
	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector BillboardLocation = GetComponentLocation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(BillboardLocation, CameraLocation);
	SetWorldRotation(LookAtRot);
}

