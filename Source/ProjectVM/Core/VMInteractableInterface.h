#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VMInteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UVMInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IVMInteractableInterface
{
	GENERATED_BODY()

public:

	/** 실제 상호작용 실행 */
	virtual void Interact() = 0;
};
