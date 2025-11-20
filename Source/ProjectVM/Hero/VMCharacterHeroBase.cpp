// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMCharacterHeroBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VMAnimInstance.h"

#include "Hero/VMHeroStatComponent.h"
#include "Hero/VMHeroSkillComponent.h"

#include "NPC/VMNPC.h"
#include "Quest/VMQuestManager.h"
#include "Core/InteractionManager.h"

#include "UI/Character/VMCharacterHeroHUD.h"
#include "Inventory/VMPickup.h"
#include "Inventory/VMInventoryComponent.h"

#include "Components/PawnNoiseEmitterComponent.h"

#include "Interface/VMStatChangeable.h"
#include "AOE/VMAOEHeal.h"
#include "Game/VMRPGPlayerController.h"

#include "AI/Allies/VMAllyBase.h"



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

	static ConstructorHelpers::FObjectFinder<UInputAction> RightMouseSkillActionRef(TEXT("/Game/Project/Input/Actions/IA_RightMouseSkill.IA_RightMouseSkill"));
	if (RightMouseSkillActionRef.Succeeded())
	{
		RightMouseSkillAction = RightMouseSkillActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShiftSkillActionRef(TEXT("/Game/Project/Input/Actions/IA_ShiftSkill.IA_ShiftSkill"));
	if (ShiftSkillActionRef.Succeeded())
	{
		ShiftSkillAction = ShiftSkillActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MiddleMouseSkillActionRef(TEXT("/Game/Project/Input/Actions/IA_MiddleMouseSkill.IA_MiddleMouseSkill"));
	if (MiddleMouseSkillActionRef.Succeeded())
	{
		MiddleMouseSkillAction = MiddleMouseSkillActionRef.Object;
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


#pragma region 나희영 손 묻음 ㅈㅅ
	ConstructorHelpers::FObjectFinder<UInputAction> SpawnAllyActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project/Input/Actions/IA_SpawnAlly.IA_SpawnAlly'"));
	if (SpawnAllyActionRef.Object)
	{
		SpawnAllyAction = SpawnAllyActionRef.Object;
	}
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));
#pragma endregion


	// 인벤토리 관련

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleActionRef(TEXT("/Game/Project/Input/Actions/IA_ToggelMenu.IA_ToggelMenu"));
	if (ToggleActionRef.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("QWER 여긴 오니"));
		ToggleAction = ToggleActionRef.Object;
	}


	PlayerInventory = CreateDefaultSubobject<UVMInventoryComponent>(TEXT("PlayerInventory"));

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 300.0f;

	BaseEyeHeight = 74.0f;

	// Todo: 나중에 변경해야 함.
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AVMCharacterHeroBase::HealthPointChange(float Amount, AActor* Causer)
{
	if (Causer == nullptr || Causer->IsValidLowLevel() == false) return;
	
	UE_LOG(LogTemp, Log, TEXT("%f, %s에 의한 HealthPointChange() 적용됨"), Amount, *Causer->GetName());
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

	Stat->OnSpeedChanged.AddUObject(this, &AVMCharacterHeroBase::ApplySpeed);
	Stat->OnCurrentHealthPointZero.AddUObject(this, &AVMCharacterHeroBase::Die);
}

void AVMCharacterHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


	//인벤토리
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AVMCharacterHeroBase::BeginInteract);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AVMCharacterHeroBase::EndInteract);


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Look);
	EnhancedInputComponent->BindAction(LeftMouseSkillAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::BasicSkill);
	EnhancedInputComponent->BindAction(RightMouseSkillAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::AdvancedSkill);
	EnhancedInputComponent->BindAction(ShiftSkillAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::MovementSkill);
	EnhancedInputComponent->BindAction(MiddleMouseSkillAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::UltimateSkill);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::Interact);
	EnhancedInputComponent->BindAction(DebuggingAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::DebuggingTest);
	
	
	EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::ToggleMenu);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started,this, &AVMCharacterHeroBase::ToggleInventory);
	
	//EnhancedInputComponent->BindAction(Toggle, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::BeginInteract);
	//EnhancedInputComponent->BindAction(Toggle, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::EndInteract);

	// Spawn Ally
	EnhancedInputComponent->BindAction(SpawnAllyAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::SpawnAllyActor);

	//다이얼로그
	EnhancedInputComponent->BindAction(NextTalkAction, ETriggerEvent::Triggered, this, &AVMCharacterHeroBase::NextTalk);

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

void AVMCharacterHeroBase::ApplySpeed(int32 SpeedStat)
{
	GetCharacterMovement()->MaxAcceleration = 500.f + SpeedStat;
	GetCharacterMovement()->MaxWalkSpeed = 500.f + SpeedStat;
}

void AVMCharacterHeroBase::Die()
{
	UE_LOG(LogTemp, Log, TEXT("Die : 테스트"));
	CurState = EHeroState::Dead;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnHeroDeath.Broadcast();
}

void AVMCharacterHeroBase::BasicSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;

	CurState = EHeroState::Skill;
	Skills->ExecuteBasicSkill(this, Stat);
}

void AVMCharacterHeroBase::AdvancedSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	
	Skills->ExecuteAdvancedSkill(this, Stat);
}

void AVMCharacterHeroBase::MovementSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	
	Skills->ExecuteMovementSkill(this, Stat);
}

void AVMCharacterHeroBase::UltimateSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	
	Skills->ExecuteUltimateSkill(this, Stat);
}

void AVMCharacterHeroBase::Interact(const FInputActionValue& Value)
{
	UGameInstance* GI = GetGameInstance();
	if (GI == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("GameInstance is nullptr"));
	}

	UInteractionManager* InteractionManager = GI->GetSubsystem<UInteractionManager>();
	if (InteractionManager == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Interaction Manager is nullptr"));
		return;
	}

	//상호작용 시작
	InteractionManager->Interact();
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

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

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

	/*	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *TraceHit.GetActor()->GetName());

			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UVMInteractionInterface::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Implements IVMInteractionInterface!"));

				const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
				UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);


			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor does NOT implement interface"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LineTrace hit nothing"));
		}*/
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
	////verify nothing has changed with the interactable state since beginning interaction
	//PerformInteractionCheck();

	//if (InteractionData.CurrentInteractable)
	//{
	//	if (IsValid(TargetInteractable.GetObject()))
	//	{
	//		TargetInteractable->BeginInteract();

	//		if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
	//		{
	//			BeingInteract();
	//		}
	//		else
	//		{
	//			GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &AVMCharacterHeroBase::BeingInteract,
	//				TargetInteractable->InteractableData.InteractionDuration, false);
	//		}
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT("BeginInteract called"));

	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentInteractable is set: %s"),
			*InteractionData.CurrentInteractable->GetName());

		if (IsValid(TargetInteractable.GetObject()))
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetInteractable is valid, calling BeginInteract on target"));
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				UE_LOG(LogTemp, Warning, TEXT("InteractionDuration ~ 0, calling BeingInteract immediately"));
				BeingInteract();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Setting timer for BeingInteract, duration: %f"),
					TargetInteractable->InteractableData.InteractionDuration);
				GetWorldTimerManager().SetTimer(
					TimerHandle_Interaction,
					this,
					&AVMCharacterHeroBase::BeingInteract,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginInteract: CurrentInteractable is nullptr"));
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

void AVMCharacterHeroBase::SetCurrentNPC(AVMNPC* NewNPC)
{
	CurrentNPC = NewNPC;
}

void AVMCharacterHeroBase::ToggleMenu()
{
	UE_LOG(LogTemp, Log, TEXT("QWER "));
	HUD->ToggleMenu();
}

void AVMCharacterHeroBase::ToggleInventory(const FInputActionValue& Value)
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetController());
	if (!PC) return;

	if (bInventoryIsOpen)
		PC->CloseInventory();
	else
		PC->OpenInventory();
}


void AVMCharacterHeroBase::OnHitExplosionByAOE(AActor* Target, FVector ExplosionCenter)
{
	if (Target != this)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("내가 몇번 호출되었게?"));

	// 폭발 중심 -> 자기 자신 방향
	FVector Direction = GetActorLocation() - ExplosionCenter;
	Direction.Z = 0;
	Direction.Normalize();

	float LaunchStrength = 1500.f;
	FVector LaunchVelocity = Direction * LaunchStrength;
	LaunchVelocity.Z = 1000.f;

	LaunchCharacter(LaunchVelocity, true, true);
}

void AVMCharacterHeroBase::OnHitMeteorByAOE(AActor* Target, float InDamage)
{
	if (Target != this)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[OnHitMeteorByAOE]: %s damage: %f"), *Target->GetName(), InDamage);

	// 데미지 입히기.
	HealthPointChange(InDamage, this);
}

void AVMCharacterHeroBase::OnHitFrozenByAOE(AActor* Target, float InDamage)
{
	if (Target != this)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[OnHitFrozenByAOE]: %s damage: %f"), *Target->GetName(), InDamage);

	ApplySlowDown();

	if (!GetWorld()->GetTimerManager().IsTimerActive(FrozenTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			FrozenTimerHandle,
			this,
			&AVMCharacterHeroBase::ClearSlowDown,
			10.0f,
			false
		);
	}

	HealthPointChange(InDamage, this);
}

void AVMCharacterHeroBase::ApplySlowDown()
{
	if (SlowFlag == true)
	{
		return;
	}
	SlowFlag = true;
	GetCharacterMovement()->MaxWalkSpeed = 200;
}

void AVMCharacterHeroBase::ClearSlowDown()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	SlowFlag = false;
}

void AVMCharacterHeroBase::OnHitThunderByAOE(AActor* Target, float InDamage)
{
	if (Target != this)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[OnHitThunderByAOE]: %s damage: %f"), *Target->GetName(), InDamage);

	GetCharacterMovement()->Velocity = FVector::Zero();
	GetCharacterMovement()->MovementMode = MOVE_None;

	//TWeakObjectPtr<AVMCharacterHeroBase> WeakHero = HeroPawn;
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		StunTimerHandle,
		[this]()
		{
			GetCharacterMovement()->MovementMode = MOVE_Walking;
		},
		10.0f,
		false
	);

	// 데미지 입히기.
	HealthPointChange(InDamage, this);
}

void AVMCharacterHeroBase::OnHitDotByAOE(AActor* Target)
{
	if (Target != this)
	{
		return;
	}

	// DOT 카운트 증가
	FireDotCount++;

	// 데미지 타이머가 안 돌고 있으면 시작
	if (!GetWorld()->GetTimerManager().IsTimerActive(DamageHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			DamageHandle,
			this,
			&AVMCharacterHeroBase::ApplyFireDotDamage,
			1.0f,        // 1초마다 실행
			true
		);
	}

	// 10초 디버프 갱신 타이머 리셋
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&AVMCharacterHeroBase::ClearFireDot,
		10.0f,
		false
	);

	UE_LOG(LogTemp, Warning, TEXT("Fire DOT Hit! Count = %d"), FireDotCount);
}

void AVMCharacterHeroBase::ApplyFireDotDamage()
{
	// 예: DOT 수 × 5 데미지
	int Damage = FireDotCount * 5;

	UE_LOG(LogTemp, Warning, TEXT("Fire DOT Damage: %d"), Damage);

	HealthPointChange(Damage, this);
	//DamageHandle.Invalidate();
	// 여기에 실제 데미지 입히는 코드 넣으면 됨
	// ApplyDamage(this, Damage, ...);
}

void AVMCharacterHeroBase::ClearFireDot()
{
	FireDotCount = 0;

	GetWorld()->GetTimerManager().ClearTimer(DamageHandle);

	UE_LOG(LogTemp, Warning, TEXT("Fire DOT expired! Count reset to 0"));
}

#pragma region 필요해서 넣었습니다
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"  // 기본 플레이어 캐릭터
#include "AI/VMEnemyBase.h"

void AVMCharacterHeroBase::SpawnAllyActor()
{
	
	
	FVector Origin = GetActorLocation();       // 공격 시작 위치
	FVector Forward = GetActorForwardVector(); // 공격 방향
	float Radius = 500.f;                       // 공격 범위
	float AngleDeg = 60.f;                      // 부채꼴 각도

	int32 Segments = 20;  // 부채꼴 외곽 디테일
	TArray<FVector> Points;
	Points.Add(Origin);

	for (int32 i = 0; i <= Segments; i++)
	{
		float CurrentAngle = FMath::Lerp(-AngleDeg / 2, AngleDeg / 2, i / (float)Segments);
		FVector Dir = Forward.RotateAngleAxis(CurrentAngle, FVector::UpVector);
		Points.Add(Origin + Dir * Radius);
	}

	// 부채꼴 외곽선 연결
	for (int32 i = 1; i < Points.Num(); i++)
	{
		DrawDebugLine(GetWorld(), Points[i - 1], Points[i], FColor::Red, false, 3.f, 0, 2.f);
		DrawDebugLine(GetWorld(), Origin, Points[i], FColor::Red, false, 3.f, 0, 2.f);
	}


	// 월드에서 플레이어 모두 가져오기
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVMEnemyBase::StaticClass(), PlayerActors);

	for (AActor* Player : PlayerActors)
	{
		FVector ToPlayer = Player->GetActorLocation() - Origin;
		float Dist = ToPlayer.Size();
		UE_LOG(LogTemp, Display, TEXT("Player: Name:%s"), *Player->GetName());
		if (Dist > Radius) continue; // 범위 밖이면 제외

		ToPlayer.Normalize();
		float Dot = FVector::DotProduct(Forward, ToPlayer);
		float CosAngle = FMath::Cos(FMath::DegreesToRadians(AngleDeg / 2));

		if (Dot >= CosAngle)
		{
			IVMStatChangeable* IStat = Cast<IVMStatChangeable>(Player);
			IStat->HealthPointChange(4, this);
			// 부채꼴 안에 있음 → 공격 적용
			//ApplyAttack(Player);
			UE_LOG(LogTemp, Log, TEXT("Hi"));
		}
	}
	return;

	AVMAOEHeal* HealSpawnedActor = GetWorld()->SpawnActor<AVMAOEHeal>(AVMAOEHeal::StaticClass(), GetActorLocation(), GetActorRotation());
	if (HealSpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("스폰 성공!"));
	}
	return;
	UE_LOG(LogTemp, Log, TEXT("SpawnAllyActor"));
	// 1. SpawnActor 알아보기.
	UWorld* World = GetWorld();
	if (World)
	{
		// 스폰 위치와 회전 지정
		FVector SpawnLocation = GetActorLocation() + FVector(100, 0, 0);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 실제 스폰
		AVMAllyBase* AllySpawnedActor = World->SpawnActor<AVMAllyBase>(AVMAllyBase::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		if (AllySpawnedActor)
		{
			AllySpawnedActor->SetOwnerTarget(this);
			UE_LOG(LogTemp, Warning, TEXT("스폰 성공!"));
		}
	}
}

void AVMCharacterHeroBase::Jump()
{
	Super::Jump();

	MakeNoise(1, this, GetActorLocation());
}
#pragma endregion