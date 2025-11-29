
#include "NPC/VMNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ListView.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameData/VMNPCTalkData.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Core/VMLoadManager.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Dialogue/VMNPCDialogueScreen.h"
#include "UI/VMBillboardComponent.h"
#include "UI/Dialogue/VMDialogueOption.h"
#include "UI/Common/VMGameScreen.h"
#include "UI/NPC/VMNPCHeadInfo.h"
#include "NPC/VMNPCEnums.h"
#include "Quest/VMQuestManager.h"
#include "Game/VMRPGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Quest/VMQuestTracker.h"
#include "UI/Quest/VMQuestDataObject.h"
#include "Core/InteractComponent.h"

// Sets default values
AVMNPC::AVMNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//초기 값 설정
	NPCType = ENPCType::Buzz;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 스켈레탈 메시 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> NPCMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (NPCMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(NPCMesh.Object);

		// 메시 위치 및 회전 보정 (언리얼 캐릭터의 메시 기본 오프셋)
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	Billboard = CreateDefaultSubobject<UVMBillboardComponent>(TEXT("BillboardComponent"));
	Billboard->SetupAttachment(RootComponent);

	InteractKey = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractKeyWidget"));
	InteractKey->SetupAttachment(Billboard);
	InteractKey->SetWorldLocation(FVector(0.0f, 0.0f, 130.0f));
	InteractKey->SetWidgetSpace(EWidgetSpace::World);
	InteractKey->SetDrawSize(FVector2D(50.0f, 50.0f));
	InteractKey->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FClassFinder<UUserWidget> InterectWidgetRef(TEXT("/Game/Project/UI/Common/WBP_VMInteractKeyWidget.WBP_VMInteractKeyWidget_C"));

	if (InterectWidgetRef.Succeeded())
	{
		InteractKey->SetWidgetClass(InterectWidgetRef.Class);
	}

	//NPC Head 정보 위젯 설정
	NPCHeadInfo = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCHeadInfo"));
	NPCHeadInfo->SetupAttachment(Billboard);
	NPCHeadInfo->SetWorldLocation(FVector(0.0f, 0.0f, 130.0f));
	NPCHeadInfo->SetWidgetSpace(EWidgetSpace::World);
	NPCHeadInfo->SetDrawSize(FVector2D(200.0f, 100.0f));
	NPCHeadInfo->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FClassFinder<UUserWidget> NPCHeadInfoRef(TEXT("/Game/Project/UI/NPC/WBP_VMNPCHeadInfo.WBP_VMNPCHeadInfo_C"));

	if (NPCHeadInfoRef.Succeeded())
	{
		NPCHeadInfo->SetWidgetClass(NPCHeadInfoRef.Class);
	}

	//카메라 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = false; //플레이어 회전 따라가지 않음
	CameraBoom->bDoCollisionTest = true; //벽 충돌 시 자동 당기기
	NPCCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("NPCCamera"));
	NPCCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //봉 끝에 카메라 붙임

	CameraBoom->SetRelativeRotation(FRotator(-20.0f, 130.0f, 0.0f));
	CameraBoom->TargetArmLength = 500.f;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVMNPC::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("NPC"));

	NPCId = FName(*StaticEnum<ENPCType>()->GetNameStringByValue((int64)NPCType));

	FVMNPCData* LoadedData = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetNPCDataRow(NPCId);
	if (LoadedData)
	{
		NPCData = *LoadedData; // 포인터 → 값 복사
	}

	InteractKey->SetVisibility(false);

	//퀘스트 매니저 구독
	UVMQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UVMQuestManager>();
	if (QuestManager != nullptr)
	{
		QuestManager->OnQuestPublished.RemoveAll(this);
		QuestManager->OnQuestPublished.AddUObject(
			this,
			&AVMNPC::HandleQuestPublished
		);

		QuestManager->OnQuestCompleted.RemoveAll(this);
		QuestManager->OnQuestCompleted.AddUObject(
			this,
			&AVMNPC::HandleQuestCompleted
		);
	}

	if (NPCHeadInfo != nullptr)
	{
		UVMNPCHeadInfo* NPCHeadInfoWidget = Cast<UVMNPCHeadInfo>(NPCHeadInfo->GetUserWidgetObject());
		if (NPCHeadInfoWidget != nullptr)
		{
			NPCHeadInfoWidget->NPCName->SetText(NPCData.GetNPCName());
			NPCHeadInfoWidget->QuestionIcon->SetVisibility(ESlateVisibility::Hidden);
			NPCHeadInfoWidget->ExclamationIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AVMNPC::HandleQuestPublished(const FVMQuestData& QuestData)
{
	if (QuestData.QuestGiver == NPCId)
	{
		//퀘스트가 없었는데 생긴거라면 headInfo에 QuestionIcon 띄우기
		if (AvailableQuests.IsEmpty())
		{
			UVMNPCHeadInfo* NPCHeadInfoWidget = Cast<UVMNPCHeadInfo>(NPCHeadInfo->GetUserWidgetObject());
			if (NPCHeadInfoWidget != nullptr)
			{
				NPCHeadInfoWidget->QuestionIcon->SetVisibility(ESlateVisibility::Visible);
			}
		}
		//퀘스트 배열에 추가
		AvailableQuests.Enqueue(&QuestData);
	}
}

void AVMNPC::HandleQuestCompleted(const FVMQuestData& QuestData)
{
	if (QuestData.QuestGiver == NPCId)
	{
		//퀘스트 완료가 없었는데 생긴거라면 headInfo에 ExclamationIcon 띄우기
		if (AvailableQuests.IsEmpty())
		{
			UVMNPCHeadInfo* NPCHeadInfoWidget = Cast<UVMNPCHeadInfo>(NPCHeadInfo->GetUserWidgetObject());
			if (NPCHeadInfoWidget != nullptr)
			{
				NPCHeadInfoWidget->ExclamationIcon->SetVisibility(ESlateVisibility::Visible);
			}
		}
		CompletedQuests.Enqueue(QuestData); // 값 복사 하지 않으면 휘발됨. 
	}
}

void AVMNPC::AddDialogueOption(ENPCOption NewNPCOption)
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}
	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
	if (Dialogue == nullptr)
	{
		return;
	}

	UVMDialogueOption* NewOption = NewObject<UVMDialogueOption>(this);
	NewOption->OptionType = NewNPCOption;

	switch (NewNPCOption)
	{
	case ENPCOption::Talk:
		NewOption->OptionText = TEXT("대화하기");
		break;

	case ENPCOption::Quest:
		NewOption->OptionText = TEXT("퀘스트 받기");
		break;

	case ENPCOption::QuestClear:
		NewOption->OptionText = TEXT("퀘스트 완료");
		break;

	case ENPCOption::Shop:
		NewOption->OptionText = TEXT("상점");
		break;

	case ENPCOption::Exit:
		NewOption->OptionText = TEXT("나가기");
		break;

	case ENPCOption::Accept:
		NewOption->OptionText = TEXT("수락");
		break;

	case ENPCOption::Decline:
		NewOption->OptionText = TEXT("거절");
		break;

	default:
		NewOption->OptionText = TEXT("Unknown");
		break;
	}

	//NewOption->OptionText = StaticEnum<ENPCOption>()->GetDisplayNameTextByValue((int64)NewNPCOption).ToString();
	NewOption->OwnerNPC = this;
	Dialogue->DialogueOptionList->AddItem(NewOption);

}

void AVMNPC::SetDialogueOption()
{
	AddDialogueOption(ENPCOption::Talk);
	if (!AvailableQuests.IsEmpty())
	{
		AddDialogueOption(ENPCOption::Quest);
	}
	if (!CompletedQuests.IsEmpty())
	{
		AddDialogueOption(ENPCOption::QuestClear);
	}
	AddDialogueOption(ENPCOption::Exit);
}

void AVMNPC::TalkSetting(FString TalkType)
{
	DialogueTexts.Empty();
	int32 TalkIndex = 0;
	FVMNPCTalkData* VMNPCTalk;
	do
	{
		FName TalkTag = FName(FString::Printf(TEXT("%s-%s-%03d"), *NPCId.ToString(), *TalkType, ++TalkIndex));
		VMNPCTalk = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetNPCTalkRow(TalkTag);
		if (VMNPCTalk == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Talk Row Not Found: %s"), *TalkTag.ToString());
			break;
		}
		DialogueTexts.Add(*VMNPCTalk);
	} while (!VMNPCTalk->bIsLastLine);
}

void AVMNPC::TurnToPlayer()
{
	// 플레이어 방향으로 NPC 회전
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}
	APawn* PlayerPawn = PC->GetPawn();
	if (PlayerPawn)
	{
		FVector NPCPos = GetActorLocation();
		FVector PlayerPos = PlayerPawn->GetActorLocation();

		// Z축 차이는 제거해서 고개 들고 숙이는 회전 없애기
		PlayerPos.Z = NPCPos.Z;

		FRotator LookAtRot = (PlayerPos - NPCPos).Rotation();
		SetActorRotation(LookAtRot);
	}
}


// Called every frame
void AVMNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVMNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVMNPC::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact"));

	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}
	APawn* PlayerPawn = PC->GetPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}
	AVMCharacterHeroBase* HeroBase = Cast<AVMCharacterHeroBase>(PlayerPawn);
	if (HeroBase == nullptr)
	{
		return;
	}
	HeroBase->ChangeInputMode(EInputMode::Dialogue);
	HeroBase->SetCurrentNPC(this);
	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
	if (Dialogue == nullptr)
	{
		return;
	}

	TalkSetting(*NPCData.GetStartTalkId().ToString());

	Dialogue->SetNPCName(NPCData.GetNPCName());
	Dialogue->DialogueOptionList->ClearListItems();

	SetDialogueOption();

	PC->ShowScreen(EScreenUIType::DialogueScreen);

	CurrentDialogueIndex = 0;
	NextDialogue();

	TurnToPlayer();
	PC->SetViewTargetWithBlend(this, 0.6f);
}

bool AVMNPC::NextDialogue()
{
	NPCState = ENPCState::Talk;
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return false;
	}
	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
	if (Dialogue == nullptr)
	{
		return false;
	}

	if (CurrentDialogueIndex < DialogueTexts.Num())
	{
		Dialogue->SetNPCText(FText::FromString(DialogueTexts[CurrentDialogueIndex].Dialogue));
		++CurrentDialogueIndex;

		if (DialogueTexts.Num() == CurrentDialogueIndex)
		{
			Dialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Visible);

			if (PC != nullptr)
			{
				NPCState = ENPCState::Idle;
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(Dialogue->DialogueOptionList->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%d lll %d"), DialogueTexts.Num(), CurrentDialogueIndex);
		}

		return true;
	}
	return false;
}

void AVMNPC::StartQuest()
{
	UE_LOG(LogTemp, Log, TEXT("Start Quest"));
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}
	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
	if (Dialogue == nullptr)
	{
		return;
	}

	CurrentDialogueIndex = 0; // Reset for next time
	//PC->ShowScreen(EScreenUIType::GameScreen);
	Dialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Hidden);

	PC->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	const FVMQuestData* QuestData = *AvailableQuests.Peek();
	TalkSetting(QuestData->QuestId.ToString());

	//다이얼로그 옵션 초기화
	Dialogue->DialogueOptionList->ClearListItems();
	AddDialogueOption(ENPCOption::Accept);
	AddDialogueOption(ENPCOption::Decline);

	NextDialogue();
}

void AVMNPC::StartDailyTalk()
{
	UE_LOG(LogTemp, Log, TEXT("Start Daily Talk"));
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}
	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
	if (Dialogue == nullptr)
	{
		return;
	}


	CurrentDialogueIndex = 0; // Reset for next time

	Dialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Hidden);

	PC->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	int32 RandomIndex = FMath::RandRange(1, DailyTalkCount);
	FString NewTalkType = FString::Printf(TEXT("Daily%02d"), RandomIndex);
	TalkSetting(NewTalkType);

	Dialogue->DialogueOptionList->ClearListItems();

	SetDialogueOption();

	NextDialogue();
}

void AVMNPC::EndDialogue()
{
	UE_LOG(LogTemp, Log, TEXT("End Dialogue"));
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}
	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
	if (Dialogue == nullptr)
	{
		return;
	}

	//Dialogue->SetVisibility(ESlateVisibility::Hidden);
	PC->ShowScreen(EScreenUIType::GameScreen);
	CurrentDialogueIndex = 0; // Reset for next time
	Dialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Hidden);

	PC->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter != nullptr)
	{
		AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(PlayerCharacter);
		if (Player != nullptr)
		{
			Player->ChangeInputMode(EInputMode::Default);
		}
	}

	//시점 다시 돌리기
	PC->SetViewTargetWithBlend(PC->GetPawn(), 0.5f);
}

void AVMNPC::QuestCompleted()
{
	FVMQuestData QuestData;
	if (CompletedQuests.Dequeue(QuestData))
	{
		//퀘스트 완료 이후 더 이상 퀘스트 완료가 없다면 headInfo에서 ExclamationIcon 숨기기
		if (AvailableQuests.IsEmpty())
		{
			UVMNPCHeadInfo* NPCHeadInfoWidget = Cast<UVMNPCHeadInfo>(NPCHeadInfo->GetUserWidgetObject());
			if (NPCHeadInfoWidget != nullptr)
			{
				NPCHeadInfoWidget->ExclamationIcon->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		EndDialogue();

		GetGameInstance()->GetSubsystem<UVMQuestManager>()->ClearQuest(QuestData);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			AVMRPGPlayerController* VMPC = Cast<AVMRPGPlayerController>(PC);
			if (VMPC)
			{
				VMPC->ShowQuestClearUI();   // ⭐ 호출
			}

			// 2초 뒤 UI 숨기기 타이머 설정
			FTimerHandle HideUITimer;
			GetWorld()->GetTimerManager().SetTimer(
				HideUITimer,
				FTimerDelegate::CreateLambda([VMPC]()
					{
						if (VMPC)
						{
							VMPC->HideQuestClearUI();
						}
					}),
				2.0f,
				false
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("큐가 비어있습니다."));
	}
}

void AVMNPC::AcceptQuest()
{
	UE_LOG(LogTemp, Log, TEXT("Accept Quest"));
	const FVMQuestData* Test = *AvailableQuests.Peek();
	UE_LOG(LogTemp, Log, TEXT("bb%s"), *(Test->QuestGiver).ToString());

	const FVMQuestData* QuestData = nullptr;
	AvailableQuests.Dequeue(QuestData);

	//퀘스트 수락 이후 더 이상 퀘스트가 없다면 headInfo에서 QuestionIcon 숨기기
	if (AvailableQuests.IsEmpty())
	{
		UVMNPCHeadInfo* NPCHeadInfoWidget = Cast<UVMNPCHeadInfo>(NPCHeadInfo->GetUserWidgetObject());
		if (NPCHeadInfoWidget != nullptr)
		{
			NPCHeadInfoWidget->QuestionIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	EndDialogue();

	GetGameInstance()->GetSubsystem<UVMQuestManager>()->AcceptQuest(*QuestData);
}

void AVMNPC::EnterShop()
{
	//자식에서 구현
}
