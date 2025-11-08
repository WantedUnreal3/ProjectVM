// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVMCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/BoxComponent.h"

#include "Projectile/VMStraightProjectile.h"
#include "Projectile/VMHomingProjectile.h"
#include "AI/Allies/VMAllyBase.h"
#include "AI/VMEnemyBase.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjectVMCharacter
AProjectVMCharacter::AProjectVMCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

#pragma region PawnSensingSection
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));
#pragma endregion 

#pragma region CollisionSection
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
#pragma endregion 

#pragma region StatSection
	SetCurrentHP(100.0f);
#pragma endregion 

#pragma region Mesh설정
	/*ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Meshes/Serath.Serath'"));
	if (SkeletonMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletonMeshRef.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonSerath/Characters/Heroes/Serath/Serath_AnimBlueprint.Serath_AnimBlueprint_C'"));
	if (AnimRef.Class)
	{
		GetMesh()->SetAnimClass(AnimRef.Class);
	}*/
#pragma endregion 

#pragma region InputActionSection
	ConstructorHelpers::FObjectFinder<UInputAction> LeftRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_LeftButton.IA_LeftButton'"));
	if (LeftRef.Object)
	{
		LeftMouseAction = LeftRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> RightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_RightButton.IA_RightButton'"));
	if (RightRef.Object)
	{
		RightMouseAction = RightRef.Object;
	}
#pragma endregion 

	ConstructorHelpers::FObjectFinder<UInputAction> SpawnAllyActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_SpawnAlly.IA_SpawnAlly'"));
	if (SpawnAllyActionRef.Object)
	{
		SpawnAllyAction = SpawnAllyActionRef.Object;
	}
}

void AProjectVMCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectVMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AProjectVMCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectVMCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectVMCharacter::Look);

		// LeftAttack
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Triggered, this, &AProjectVMCharacter::LeftAttack);
		
		// RightAttack
		EnhancedInputComponent->BindAction(RightMouseAction, ETriggerEvent::Triggered, this, &AProjectVMCharacter::RightAttack);

		// SpawnAlly
		EnhancedInputComponent->BindAction(SpawnAllyAction, ETriggerEvent::Triggered, this, &AProjectVMCharacter::SpawnAllyActor);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectVMCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProjectVMCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectVMCharacter::Jump()
{
	Super::Jump();

	UE_LOG(LogTemp, Log, TEXT("(%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	MakeNoise(1.0f, this, GetActorLocation());  // 점프할 때 소리 발생
}


void AProjectVMCharacter::LeftAttack()
{
	UE_LOG(LogTemp, Log, TEXT("LeftAttack"));

	FTransform spawnTransform = GetActorTransform();

	AVMStraightProjectile* Projectile = GetWorld()->SpawnActor<AVMStraightProjectile>(AVMStraightProjectile::StaticClass(), spawnTransform);
}

void AProjectVMCharacter::RightAttack()
{
	UE_LOG(LogTemp, Log, TEXT("RightAttack"));

	FTransform spawnTransform = GetActorTransform();



	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;               // <<-- 중요: 소유자 미리 지정
	SpawnParams.Instigator = GetInstigator();

	AVMHomingProjectile* Projectile = GetWorld()->SpawnActor<AVMHomingProjectile>(
		AVMHomingProjectile::StaticClass(),
		spawnTransform,
		SpawnParams
	);
	if (Projectile)
	{
		AActor* TargetEnemy = FindClosestEnemy(); // 직접 구현 필요
		if (TargetEnemy)
		{
			Projectile->SetTarget(TargetEnemy);
		}
	}
}

AActor* AProjectVMCharacter::FindClosestEnemy()
{
	AActor* ClosestEnemy = nullptr;
	float MinDistance = FLT_MAX;

	for (TActorIterator<AVMEnemyBase> It(GetWorld()); It; ++It)
	{
		float Distance = FVector::Dist(GetActorLocation(), It->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestEnemy = *It;
		}
	}

	return ClosestEnemy;
}

void AProjectVMCharacter::SpawnAllyActor()
{
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
