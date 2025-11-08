// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMPlayer.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

#include "Macro/VMAIMarco.h"

#include "Engine/HitResult.h"
#include "Engine/OverlapResult.h"

#include "Engine/DamageEvents.h"
#include "AI/Allies/VMAllyBase.h"

#include "Components/InputComponent.h"

//#include "GameFramework/Controller.h"
//#include "InputActionValue.h"
//#include "Components/PawnNoiseEmitterComponent.h"
//#include "Components/BoxComponent.h"
//
//#include "Projectile/VMStraightProjectile.h"
//#include "Projectile/VMHomingProjectile.h"
//#include "AI/VMEnemyBase.h"
//#include "EngineUtils.h"

AVMPlayer::AVMPlayer()
{
	// Tick On 
	PrimaryActorTick.bCanEverTick = true;

#pragma region Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
#pragma endregion

#pragma region CapsuleCollision
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("VMCAPSULE"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
#pragma endregion

#pragma region CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
#pragma endregion

#pragma region Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
#pragma endregion

#pragma region Mesh설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Meshes/Serath.Serath'"));
	if (SkeletonMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletonMeshRef.Object);
	}

	// TODO: 변경해야할 듯. -> 내가 만든게 아님.
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonSerath/Characters/Heroes/Serath/Serath_AnimBlueprint.Serath_AnimBlueprint_C'"));
	if (AnimRef.Class)
	{
		GetMesh()->SetAnimClass(AnimRef.Class);
	}
#pragma endregion 

#pragma region InputActionSection
	ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (DefaultMappingContextRef.Object)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}
	

	ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}


	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}


	ConstructorHelpers::FObjectFinder<UInputAction> LeftRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_LeftButton.IA_LeftButton'"));
	if (LeftRef.Object)
	{
		LeftMouseAction = LeftRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> MiddleRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_MiddleButton.IA_MiddleButton'"));
	if (MiddleRef.Object)
	{
		MiddleMouseAction = MiddleRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> RightRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_RightButton.IA_RightButton'"));
	if (RightRef.Object)
	{
		RightMouseAction = RightRef.Object;
	}
#pragma endregion 


#pragma region MontageSection
	ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/VM_Attack.VM_Attack'"));
	AttackMontage = AttackMontageRef.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> LaserAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/VM_LaserAttack.VM_LaserAttack'"));
	LaserAttackMontage = LaserAttackMontageRef.Object;
#pragma endregion 

	ConstructorHelpers::FObjectFinder<UInputAction> SpawnAllyActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_SpawnAlly.IA_SpawnAlly'"));
	if (SpawnAllyActionRef.Object)
	{
		SpawnAllyAction = SpawnAllyActionRef.Object;
	}



	MaxHp = 100;
	CurrentHp = 75;

	MaxMp = 100;
	CurrentMp = 50;

}

void AVMPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVMPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVMPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AVMPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVMPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVMPlayer::Look);

		// LeftAttack
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Triggered, this, &AVMPlayer::LeftAttack);

		// MiddleAttack
		EnhancedInputComponent->BindAction(MiddleMouseAction, ETriggerEvent::Triggered, this, &AVMPlayer::MiddleAttack);
		
		// RightAttack
		EnhancedInputComponent->BindAction(RightMouseAction, ETriggerEvent::Triggered, this, &AVMPlayer::RightAttack);

		// SpawnAlly
		EnhancedInputComponent->BindAction(SpawnAllyAction, ETriggerEvent::Triggered, this, &AVMPlayer::SpawnAllyActor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AVMPlayer::PlayAttackAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensureAlways(AnimInstance);
	if (AnimInstance)
	{
		const float AnimPlayRate = 1.0f;
		AnimInstance->Montage_Play(AttackMontage, AnimPlayRate);
	}
}

void AVMPlayer::PlayLaserAttackAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ensureAlways(AnimInstance);
	if (AnimInstance)
	{
		const float AnimPlayRate = 2.0f;
		AnimInstance->Montage_Play(LaserAttackMontage, AnimPlayRate);
	}
}

void AVMPlayer::LaserAttackHitCheck()
{
	FHitResult OutHitResult;
	TArray<FHitResult> HitResults;
	//TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(LaserAttack), false, this);
	

	const float AttackRange = 1000.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//bool Result = GetWorld()->LineTraceMultiByChannel(OverlapResults, Start, End, ECC_GameTraceChannel1, Params);
	bool Result = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (Result || HitResults.Num())
	{
		for (auto HitResult : HitResults)
		{

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(1, DamageEvent, GetController(), this);
			UE_LOG(LogTemp, Log, TEXT("Name: %s"), *HitResult.GetActor()->GetName());
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Empty"));

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor Color = Result ? FColor::Green: FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), Color, false, 5.0f);
	
	//DrawDebugLine(GetWorld(), Start, End, Color, false, 2.0f, 0, 2.0f);
}

void AVMPlayer::Move(const FInputActionValue& Value)
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

void AVMPlayer::Look(const FInputActionValue& Value)
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

void AVMPlayer::Jump()
{
	Super::Jump();

	UE_LOG(LogTemp, Log, TEXT("(%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	MakeNoise(1.0f, this, GetActorLocation());  // 점프할 때 소리 발생
}


void AVMPlayer::LeftAttack()
{
	UE_LOG(LogTemp, Log, TEXT("LeftAttack"));
	
	PlayAttackAnim();
	/*FTransform spawnTransform = GetActorTransform();

	AVMStraightProjectile* Projectile = GetWorld()->SpawnActor<AVMStraightProjectile>(AVMStraightProjectile::StaticClass(), spawnTransform);*/
}

void AVMPlayer::MiddleAttack()
{
	UE_LOG(LogTemp, Log, TEXT("MiddleAttack"));

	PlayLaserAttackAnim();
}

void AVMPlayer::RightAttack()
{
	UE_LOG(LogTemp, Log, TEXT("RightAttack"));


	PlayAttackAnim();
	//FTransform spawnTransform = GetActorTransform();



	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;               // <<-- 중요: 소유자 미리 지정
	//SpawnParams.Instigator = GetInstigator();

	//AVMHomingProjectile* Projectile = GetWorld()->SpawnActor<AVMHomingProjectile>(
	//	AVMHomingProjectile::StaticClass(),
	//	spawnTransform,
	//	SpawnParams
	//);
	//if (Projectile)
	//{
	//	AActor* TargetEnemy = FindClosestEnemy(); // 직접 구현 필요
	//	if (TargetEnemy)
	//	{
	//		Projectile->SetTarget(TargetEnemy);
	//	}
	//}
}

AActor* AVMPlayer::FindClosestEnemy()
{
	AActor* ClosestEnemy = nullptr;
	/*float MinDistance = FLT_MAX;

	for (TActorIterator<AVMEnemyBase> It(GetWorld()); It; ++It)
	{
		float Distance = FVector::Dist(GetActorLocation(), It->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestEnemy = *It;
		}
	}*/

	return ClosestEnemy;
}

void AVMPlayer::SpawnAllyActor()
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
			// TODO: 확인해야함. 왜 꼭 줘야하는지
			AllySpawnedActor->SetOwnerTarget(this);
			UE_LOG(LogTemp, Warning, TEXT("스폰 성공!"));
		}
	}
}
