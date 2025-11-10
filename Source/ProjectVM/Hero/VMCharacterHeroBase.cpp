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

AVMCharacterHeroBase::AVMCharacterHeroBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

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
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

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

	static ConstructorHelpers::FObjectFinder<UInputAction> NextTalkActionRef(TEXT("/Game/Project/Input/Actions/IA_NextTalk.IA_NextTalk"));
	if (NextTalkActionRef.Succeeded())
	{
		NextTalkAction = NextTalkActionRef.Object;
	}

	Stat = CreateDefaultSubobject<UVMHeroStatComponent>(TEXT("Stat"));
	Skills = CreateDefaultSubobject<UVMHeroSkillComponent>(TEXT("Skills"));
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
