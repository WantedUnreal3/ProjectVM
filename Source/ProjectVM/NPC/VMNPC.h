// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VMNPCEnums.h"
#include "GameData/VMNPCData.h"
#include "GameData/VMQuestData.h"
#include "Core/VMInteractableInterface.h"
#include "VMNPC.generated.h"

UCLASS()
class PROJECTVM_API AVMNPC : public ACharacter, public IVMInteractableInterface
{
	GENERATED_BODY()

public:
	AVMNPC();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact() override;
	bool NextDialogue();

	//옵션 호출 함수
	void StartQuest();
	void QuestCompleted();
	void StartDailyTalk();
	void EndDialogue();
	void AcceptQuest();
	virtual void EnterShop(); //상점 호출 함수, 자식에서 구현

protected:
	virtual void BeginPlay() override;

	void HandleQuestPublished(const FVMQuestData& QuestData);
	void HandleQuestCompleted(const FVMQuestData& QuestData);

	//다이얼로그에 옵션 추가하는 함수
	void AddDialogueOption(ENPCOption NewNPCOption);
	virtual void SetDialogueOption();

	void TalkSetting(FString TalkType);

	virtual void TurnToPlayer();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UCameraComponent> NPCCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UWidgetComponent> InteractKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UWidgetComponent> NPCHeadInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UVMBillboardComponent> Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ENPCState NPCState = ENPCState::Idle;

protected:

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	ENPCType NPCType;

	UPROPERTY()
	FVMNPCData NPCData;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName NPCId;
	
	//UPROPERTY(VisibleAnywhere)
	//class UBoxComponent* InteractKeyBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UInteractComponent> InteractComponent;

	//대화 데이터 포인터로 저장
	TArray<struct FVMNPCTalkData*> DialogueTexts;
	int32 CurrentDialogueIndex = 0;

	//일상 대화 총 갯수 
	static constexpr int32 DailyTalkCount = 5;

	TQueue<const struct FVMQuestData*> AvailableQuests; //수락 가능한 퀘스트
	TQueue< struct FVMQuestData> CompletedQuests; //완료된 퀘스트
};
