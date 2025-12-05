// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Core/GameEnums.h"
#include "Core/VMHeroEnums.h"

#include "Interface/VMStatChangeable.h"
#include "Interface/VMInteractionInterface.h"

#include "Inventory/VMInventoryComponent.h"
#include "UI/Character/VMCharacterHeroHUD.h"
#include "Item/Equipment/VMEquipmentInfo.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "VMCharacterHeroBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FHeroDeathHandler);

class UVMEquipment;
class UVMHeroStatComponent;
class UVMEquipmentComponent;


// 인벤토리 관련 구조체
USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	float LastInteractionCheckTime;
};



UCLASS()
class PROJECTVM_API AVMCharacterHeroBase : public ACharacter, public IVMStatChangeable
{
	GENERATED_BODY()

public:
	AVMCharacterHeroBase();
	
	FHeroDeathHandler OnHeroDeath;
	FHeroDeathHandler OnHeroResurrect;

	FORCEINLINE class UCameraComponent* GetCameraComponent() { return FollowCamera; }
	FORCEINLINE class UVMHeroStatComponent* GetStatComponent() { return Stat; }
	FORCEINLINE class UVMHeroSkillComponent* GetSkillComponent() { return Skills; }
	FORCEINLINE EHeroState GetHeroState() { return CurState; }
	FORCEINLINE void SetHeroState(EHeroState InState) { CurState = InState; }
	void Resurrect();
	
	void ChangeInputMode(EInputMode NewMode);
	virtual void HealthPointChange(float Amount, AActor* Causer) override; // TODO : ApplyDamage로 네이밍 변경 고려
	void SetInteractNPC(class AVMNPC* NewInteractNPC);

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };
	FORCEINLINE UVMInventoryComponent* GetInventory() const { return PlayerInventory; }
	void UpdateInteractionWidget() const;
	void DropItem(UVMEquipment* ItemToDrop, const int32 QuantityToDrop);

	void SetCurrentNPC(AVMNPC* NewNPC);

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ApplySpeed(int32 SpeedStat);
	void Die();
	
	void BasicSkill(const FInputActionValue& Value);
	void AdvancedSkill(const FInputActionValue& Value);
	void MovementSkill(const FInputActionValue& Value);
	void UltimateSkill(const FInputActionValue& Value);

	//상호작용
	void Interact(const FInputActionValue& Value);
	//대화 넘기기
	void NextTalk(const FInputActionValue& Value);
	void DebuggingTest(const FInputActionValue& Value);

	// 인벤토리 관련 함수
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void ToggleMenu();


	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 죄송합니다. 손 좀 대겠습니다.
public:
	UFUNCTION()
	void OnHitExplosionByAOE(AActor* Target, FVector ExplosionCenter);
	
	UFUNCTION()
	void OnHitMeteorByAOE(AActor* Target, float InDamage);

	UFUNCTION()
	void OnHitFrozenByAOE(AActor* Target, float InDamage);
	
	bool SlowFlag = false;
	void ApplySlowDown();
	void ClearSlowDown();
	float MaxWalkSpeed;

	UFUNCTION()
	void OnHitThunderByAOE(AActor* Target, float InDamage);

	FTimerHandle StunTimerHandle;

	UFUNCTION()
	void OnHitDotByAOE(AActor* Target);

	void ClearFireDot();
	void ApplyFireDotDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member")
	int32 FireDotCount = 0;
	
	FTimerHandle FireTimerHandle;
	FTimerHandle FrozenTimerHandle;
	FTimerHandle DamageHandle;

	UFUNCTION()
	void ToggleInventory(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<AVMCharacterHeroHUD> HUD;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DeadStateInputMappingContext;

	//대화 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DialogueMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LeftMouseSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RightMouseSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShiftSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MiddleMouseSkillAction;

	//E 상호작용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	//P 디버깅용 테스트 키 입력 (이상윤)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DebuggingAction;

	//NPC 다이얼 로그 넘기기 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> NextTalkAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UVMHeroStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UVMHeroSkillComponent> Skills;

	// 인벤토리 관련 변수
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	TObjectPtr<UVMInventoryComponent> PlayerInventory;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IVMInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	EHeroState CurState;

	//상호작용 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bCanInteract = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	class AVMNPC* CurrentNPC = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	bool bInventoryIsOpen = false;


#pragma region 나희영_손 묻음 ㅈㅅ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SpawnAllyAction;

	void SpawnAllyActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Noise, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnNoiseEmitterComponent> PawnNoiseEmitter;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	float InteractionCheckFrequency;
	float InteractionCheckDistance;
	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DieMontage;
};
