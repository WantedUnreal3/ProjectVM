
#include "Portal/VMFieldPortal.h"
#include "Core/VMLevelManager.h"

AVMFieldPortal::AVMFieldPortal()
{
	PlayerTeleportPos = FVector(-4000.f, -900.f, 500.f);
}

void AVMFieldPortal::Interact()
{
	Super::Interact();

	TeleportPlayerToMap();
	UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();
	LevelManager->DeleteLevel(DeleteLevelName);
}
