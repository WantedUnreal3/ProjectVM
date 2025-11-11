// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VMNPCEnums.h"
#include "GameData/VMNPCData.h"
#include "VMNPC.generated.h"

UCLASS()
class PROJECTVM_API AVMNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVMNPC();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnInteractTriggerOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnInteractTriggerOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	virtual void Interact();
	bool NextDialogue();

	UFUNCTION()
	void StartQuest();

	UFUNCTION()
	void QuestCompleted();

	UFUNCTION()
	void StartDailyTalk();
	UFUNCTION()
	void EndDialogue();
	UFUNCTION()
	void AcceptQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//다이얼로그에 들어갈 옵션 정하는 함수
	//virtual void SelectDialogueOption();

	//다이얼로그에 옵션 추가하는 함수
	void AddDialogueOption(ENPCOption NewNPCOption);

	void TalkSetting(FString TalkType);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UWidgetComponent> InteractKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UVMBillboardComponent> Billboard;

protected:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	ENPCType NPCType;

	UPROPERTY()
	FVMNPCData NPCData;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FName NPCId;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* InteractKeyBoxComponent;

	//대화 위젯
	//class UVMNPCDialogueScreen* VMNPCDialogue;
	//TSubclassOf<class UVMNPCDialogueScreen> VMNPCDialogueClass;

	//대화 데이터 포인터로 저장
	TArray<struct FVMNPCTalkData*> DialogueTexts;
	int32 CurrentDialogueIndex = 0;

	//일상 대화 총 갯수 
	static constexpr int32 DailyTalkCount = 5;

	TQueue<const struct FVMQuestData*> AvailableQuests; //수락 가능한 퀘스트
	TQueue< struct FVMQuestData> CompletedQuests; //완료된 퀘스트
};
