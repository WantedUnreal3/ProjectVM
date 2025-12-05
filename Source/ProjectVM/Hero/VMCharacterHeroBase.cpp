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
#include "UI/Inventory/VMInventoryPanel.h"
#include "UI/Inventory/VMEquipmentPanel.h"

#include "Hero/HeroStat.h"

#include "Components/PawnNoiseEmitterComponent.h"

#include "Interface/VMStatChangeable.h"
#include "AOE/VMAOEHeal.h"
#include "Game/VMRPGPlayerController.h"

#include "AI/Allies/VMAllyBase.h"
#include "Core/VMLevelManager.h"
#include "Macro/VMPhysics.h"

AVMCharacterHeroBase::AVMCharacterHeroBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(VM_HERO_COLLISION);

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
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-30.0f, 0.0f, 0.0f));
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Project/Input/IMC_Shoulder.IMC_Shoulder"));
	if (InputMappingContextRef.Succeeded())
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DeadStateInputMappingContextRef(TEXT("/Game/Project/Input/IMC_Dead.IMC_Dead"));
	if (DeadStateInputMappingContextRef.Succeeded())
	{
		DeadStateInputMappingContext = DeadStateInputMappingContextRef.Object;
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


	ConstructorHelpers::FObjectFinder<UInputAction> SpawnAllyActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Project/Input/Actions/IA_SpawnAlly.IA_SpawnAlly'"));
	if (SpawnAllyActionRef.Object)
	{
		SpawnAllyAction = SpawnAllyActionRef.Object;
	}
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));


	// 인벤토리 관련

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleActionRef(TEXT("/Game/Project/Input/Actions/IA_ToggelMenu.IA_ToggelMenu"));
	if (ToggleActionRef.Succeeded())
	{
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
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
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
			case EInputMode::Dead:
				Subsystem->AddMappingContext(DeadStateInputMappingContext, 0);
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
	//PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AVMCharacterHeroBase::EndInteract);


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
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
	GetCharacterMovement()->MaxAcceleration = SpeedStat;
	GetCharacterMovement()->MaxWalkSpeed = SpeedStat;
}

void AVMCharacterHeroBase::Die()
{
	CurState = EHeroState::Dead;

	ChangeInputMode(EInputMode::Dead);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	
	OnHeroDeath.Broadcast();
}

void AVMCharacterHeroBase::Resurrect()
{
	UE_LOG(LogTemp, Log, TEXT("Test : Resurrect"));
	CurState = EHeroState::Idle;

	ChangeInputMode(EInputMode::Default);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("VMHeroCollision"));
	
	UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();
	LevelManager->DeleteLevel(FName("BossMap"));

	SetActorLocation(FVector(-3700.0f, 1400.0f, 200.0f));
	Stat->RecoverHealth(100);
	Stat->RecoverMana(100);

	OnHeroResurrect.Broadcast();
}

void AVMCharacterHeroBase::BasicSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	if (CurState == EHeroState::Skill) return;
	
	Skills->ExecuteBasicSkill(this, Stat);
}

void AVMCharacterHeroBase::AdvancedSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	if (CurState == EHeroState::Skill) return;
	
	Skills->ExecuteAdvancedSkill(this, Stat);
}

void AVMCharacterHeroBase::MovementSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	if (CurState == EHeroState::Skill) return;
	
	Skills->ExecuteMovementSkill(this, Stat);
}

void AVMCharacterHeroBase::UltimateSkill(const FInputActionValue& Value)
{
	if (Stat == nullptr) return;
	if (Skills == nullptr) return;
	if (CurState == EHeroState::Skill) return;
	
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
		//EndInteract();
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
			}
			else
			{
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginInteract: CurrentInteractable is nullptr"));
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
	// 1) 먼저 멤버 HUD 가 세팅되어 있는지 확인
	if (!HUD)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			HUD = PC->GetHUD<AVMCharacterHeroHUD>();
		}
	}

	if (!HUD || !HUD->InventoryPanel)
		return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
		return;

	const bool bVisible = HUD->InventoryPanel->IsVisible();

	if (bVisible)
	{
		HUD->InventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
		if (HUD->EquipmentPanel)
		{
			HUD->EquipmentPanel->SetVisibility(ESlateVisibility::Collapsed);
		}

		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
	else
	{
		HUD->InventoryPanel->SetVisibility(ESlateVisibility::Visible);
		if (HUD->EquipmentPanel)
		{
			HUD->EquipmentPanel->SetVisibility(ESlateVisibility::Visible);
		}

		PC->SetInputMode(FInputModeGameAndUI());
		PC->bShowMouseCursor = true;
	}
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

void AVMCharacterHeroBase::SpawnAllyActor()
{
	return;
}

#pragma endregion