// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMCharacterHeroBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Hero/VMHeroStatComponent.h"
#include "Hero/VMHeroSkillComponent.h"

#include "NPC/VMNPC.h"
#include "Quest/VMQuestManager.h"

#include "UI/Character/VMCharacterHeroHUD.h"
#include "Inventory/VMPickup.h"
#include "Inventory/VMInventoryComponent.h"



AVMCharacterHeroBase::AVMCharacterHeroBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("VMHeroCollision"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxAcceleration = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.1f;
	GetCharacterMovement()->GroundFriction = 8.f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));


	//HUD = CreateDefaultSubobject< AVMCharacterHeroHUD>(TEXT("HUDTEST"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 200.0f), FRotator(-30.0f, 0.0f, 0.0f));
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Project/Input/IMC_Shoulder.IMC_Shoulder"));
	if (InputMappingContextRef.Succeeded())
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DialogueMappingContextRef(TEXT("/Game/Project/Input/IMC_Dialogue.IMC_Dialogue"));
	if (DialogueMappingContextRef.Succeeded())
	{
		DialogueMappingContext = DialogueMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Project/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Project/Input/Actions/IA_Look.IA_Look"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Project/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftMouseSkillActionRef(TEXT("/Game/Project/Input/Actions/IA_LeftMouseSkill.IA_LeftMouseSkill"));
	if (LeftMouseSkillActionRef.Succeeded())
	{
		LeftMouseSkillAction = LeftMouseSkillActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionRef(TEXT("/Game/Project/Input/Actions/IA_Interact.IA_Interact"));
	if (InteractActionRef.Succeeded())
	{
		InteractAction = InteractActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DebuggingTestRef(TEXT("/Game/Project/Input/Actions/IA_Debugging.IA_Debugging"));
	if (DebuggingTestRef.Succeeded())
	{
		DebuggingAction = DebuggingTestRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> NextTalkActionRef(TEXT("/Game/Project/Input/Actions/IA_NextTalk.IA_NextTalk"));
	if (NextTalkActionRef.Succeeded())
	{
		NextTalkAction = NextTalkActionRef.Object;
	}

	Stat = CreateDefaultSubobject<UVMHeroStatComponent>(TEXT("Stat"));
	Skills = CreateDefaultSubobject<UVMHeroSkillComponent>(TEXT("Skills"));


	// 인벤토리 관련

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleActionRef(TEXT("/Game/Project/Input/Actions/IA_ToggelMenu.IA_ToggelMenu"));
	if (ToggleActionRef.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("QWER 여긴 오니"));
		ToggleAction = ToggleActionRef.Object;
	}


	PlayerInventory = CreateDefaultSubobject<UVMInventoryComponent>(TEXT("PlayerInventory"));

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;

	BaseEyeHeight = 74.0f;
}

void AVMCharacterHeroBase::HealthPointChange(float Amount, AActor* Causer)
{
	if (Causer == nullptr || Causer->IsValidLowLevel() == false) return;

	UE_LOG(LogTemp, Log, TEXT("%f, %s HealthPointChange() 적용됨"), Amount, *Causer->GetName());
	Stat->ApplyDamage(Amount);
}

void AVMCharacterHeroBase::ChangeInputMode(EInputMode NewMode)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// 현재 등록된 모든 매핑 제거
			Subsystem->ClearAllMappings();

			// 새로운 Context 추가
			switch (NewMode)
			{
			case EInputMode::Default:
				Subsystem->AddMappingContext(InputMappingContext, 0);
				break;
			case EInputMode::Dialogue:
				Subsystem->AddMappingContext(DialogueMappingContext, 0);
				break;
			default:
				break;
			}
		}
	}
}

void AVMCharacterHeroBase::SetInteractNPC(AVMNPC* NewInteractNPC)
{
	CurrentNPC = NewInteractNPC;
}

void AVMCharacterHeroBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		InputSystem->AddMappingContext(InputMappingContext, 0);
	}

	APlayerController* ControllerPtr = GetWorld()->GetFirstPlayerController();
	if (ControllerPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Controller is nullptr"));
		return;
	}
	AVMCharacterHeroHUD* HUDPtr = Cast<AVMCharacterHeroHUD>(ControllerPtr->GetHUD());
	if (HUDPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AVMCharacterHeroHUD is nullptr"));
		return;
	}
	//HUD = Cast<AVMCharacterHeroHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD = HUDPtr;
}

void AVMCharacterHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Look);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Interact);
	EnhancedInputComponent->BindAction(LeftMouseSkillAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::BasicSkill);
	EnhancedInputComponent->BindAction(DebuggingAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::DebuggingTest);
	
	
	EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::ToggleMenu);
	
	//EnhancedInputComponent->BindAction(Toggle, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::BeginInteract);
	//EnhancedInputComponent->BindAction(Toggle, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::EndInteract);

	//다이얼로그
	EnhancedInputComponent->BindAction(NextTalkAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::NextTalk);

	//인벤토리


}

void AVMCharacterHeroBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AVMCharacterHeroBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AVMCharacterHeroBase::BasicSkill(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Left Mouse Skill !"));
	
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;

	FHeroStat CurStat = Stat->GetStat();
	Skills->ExecuteBasicSkill(CurStat);
}

void AVMCharacterHeroBase::Interact(const FInputActionValue& Value)
{
	if (CurrentNPC != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("input E, show ui : %s"), *CurrentNPC->GetName());
		CurrentNPC->Interact();

		ChangeInputMode(EInputMode::Dialogue);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("input E, cannot interact"));
	}
}

void AVMCharacterHeroBase::NextTalk(const FInputActionValue& Value)
{
	if (CurrentNPC != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("input Space, next dialogue : %s"), *CurrentNPC->GetName());

		//다음 대화 불러오기, 없으면 대화 모드 종료
		if (!CurrentNPC->NextDialogue())
		{
			ChangeInputMode(EInputMode::Default);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("input A, cannot interact"));
	}
}

void AVMCharacterHeroBase::DebuggingTest(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Input Test Key"));

	//FVMNPCData* LoadedData = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetNPCDataRow(NPCId);
	GetGameInstance()->GetSubsystem<UVMQuestManager>()->NotifyMonsterDeath(EMonsterName::Warrior);
}



// 인벤토리
void AVMCharacterHeroBase::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0)
	{

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UVMInteractionInterface::StaticClass()))
			{
				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}



	NoInteractableFound();
}

void AVMCharacterHeroBase::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();

}

void AVMCharacterHeroBase::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		// hide interaction widget on the HUD
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}



void AVMCharacterHeroBase::BeginInteract()
{
	//verify nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				BeingInteract();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &AVMCharacterHeroBase::BeingInteract,
					TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void AVMCharacterHeroBase::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AVMCharacterHeroBase::BeingInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->BeingInteract(this);
	}
}


void AVMCharacterHeroBase::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AVMCharacterHeroBase::DropItem(UVMEquipment* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation({ GetActorLocation() + (GetActorForwardVector() * 50.0f) });

		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		//const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		TObjectPtr<AVMPickup> Pickup = GetWorld()->SpawnActor<AVMPickup>(AVMPickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item do drop was somehow null!"));
	}
}

void AVMCharacterHeroBase::ToggleMenu()
{
	UE_LOG(LogTemp, Log, TEXT("QWER "));
	HUD->ToggleMenu();
}