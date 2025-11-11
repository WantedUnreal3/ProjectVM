
#include "NPC/VMNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ListView.h"
#include "Components/WidgetComponent.h"
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
#include "NPC/VMNPCEnums.h"
#include "Quest/VMQuestManager.h"
#include "Game/VMRPGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Quest/VMQuestTracker.h"
#include "UI/Quest/VMQuestDataObject.h"

// Sets default values
AVMNPC::AVMNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//초기 값 설정
	NPCType = ENPCType::Bob;

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

	InteractKeyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractTrigger"));
	InteractKeyBoxComponent->SetupAttachment(GetCapsuleComponent());
	InteractKeyBoxComponent->SetBoxExtent(FVector(70.0f, 70.0f, 100.0f));
	InteractKeyBoxComponent->SetCollisionProfileName(TEXT("InteractTrigger"));

	InteractKey = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractKeyWidget"));
	InteractKey->SetupAttachment(Billboard);
	InteractKey->SetWorldLocation(FVector(0.0f, 0.0f, 130.0f));
	InteractKey->SetWidgetSpace(EWidgetSpace::World);
	InteractKey->SetDrawSize(FVector2D(50.0f, 50.0f));
	InteractKey->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FClassFinder<UUserWidget> InterectWidgetRef(TEXT("/Game/Project/UI/WBP_VMInteractKeyWidget.WBP_VMInteractKeyWidget_C"));

	if (InterectWidgetRef.Succeeded())
	{
		InteractKey->SetWidgetClass(InterectWidgetRef.Class);
	}

	//대화 위젯 클래스 로드
	//static ConstructorHelpers::FClassFinder<UVMNPCDialogueScreen> NPCDialogueRef(TEXT("/Game/Project/UI/WBP_VMNPCDialogueScreen.WBP_VMNPCDialogueScreen_C"));

	//if (NPCDialogueRef.Succeeded())
	//{
	//	VMNPCDialogueClass = NPCDialogueRef.Class;
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Not Class"));
	//}
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

	InteractKeyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVMNPC::OnInteractTriggerOverlapBegin);
	InteractKeyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AVMNPC::OnInteractTriggerOverlapEnd);
	InteractKey->SetVisibility(false);

	//퀘스트 매니저 구독
	UVMQuestManager* QuestManager = GetGameInstance()->GetSubsystem<UVMQuestManager>();
	if (QuestManager != nullptr)
	{
		QuestManager->OnQuestPublished.AddLambda([this](const FVMQuestData& QuestData)
			{
				if (QuestData.QuestGiver == NPCId)
				{
					AvailableQuests.Enqueue(&QuestData);
				}
			}
		);

		QuestManager->OnQuestCompleted.AddLambda([this](const FVMQuestData& QuestData)
			{
				if (QuestData.QuestGiver == NPCId)
				{
					CompletedQuests.Enqueue(QuestData); // 값 복사 하지 않으면 휘발됨. 
				}
			}
		);
	}
}

//void AVMNPC::SelectDialogueOption()
//{
//	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (PC == nullptr)
//	{
//		return;
//	}
//	UVMNPCDialogueScreen* Dialogue = Cast<UVMNPCDialogueScreen>(PC->GetScreen(EScreenUIType::DialogueScreen));
//	if (Dialogue == nullptr)
//	{
//		return;
//	}
//
//	Dialogue->DialogueOptionList->ClearListItems();
//
//	AddDialogueOption(ENPCOption::Talk);
//	if (!AvailableQuests.IsEmpty())
//	{
//		AddDialogueOption(ENPCOption::Quest);
//	}
//	if (!CompletedQuests.IsEmpty())
//	{
//		AddDialogueOption(ENPCOption::QuestClear);
//	}
//	AddDialogueOption(ENPCOption::Exit);
//
//}



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
	NewOption->OptionText = StaticEnum<ENPCOption>()->GetDisplayNameTextByValue((int64)NewNPCOption).ToString();
	NewOption->OwnerNPC = this;
	Dialogue->DialogueOptionList->AddItem(NewOption);

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
		DialogueTexts.Add(VMNPCTalk);
	} while (!VMNPCTalk->bIsLastLine);
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

void AVMNPC::OnInteractTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Overlapped with Player: %s"), *Player->GetName());
		InteractKey->SetVisibility(true);
		Player->SetInteractNPC(this);
	}
}

void AVMNPC::OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//만약 NPC가 붙어있어서 여러 NPC가 활성화 된 상태라면 하나만 선택된다. 바로 옆으로 이동하면 nullptr로 설정하기 때문에 다른 NPC가 실행되지는 않는다.
	//일단 NPC를 붙어서 만들지는 않을 예정이르모 이렇게 처리한다.
	if (AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Exit with Player: %s"), *Player->GetName());
		InteractKey->SetVisibility(false);
		Player->SetInteractNPC(nullptr);
	}
}

void AVMNPC::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact Quest"));
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

	TalkSetting(*NPCData.GetStartTalkId().ToString());

	Dialogue->SetNPCName(NPCData.GetNPCName());
	Dialogue->DialogueOptionList->ClearListItems();
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

	PC->ShowScreen(EScreenUIType::DialogueScreen);
	//Dialogue->SetVisibility(ESlateVisibility::Visible);
	NextDialogue();
}

bool AVMNPC::NextDialogue()
{
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
		Dialogue->SetNPCText(FText::FromString(DialogueTexts[CurrentDialogueIndex]->Dialogue));
		++CurrentDialogueIndex;

		if (DialogueTexts.Num() == CurrentDialogueIndex)
		{
			Dialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Visible);

			if (PC != nullptr)
			{
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
	AddDialogueOption(ENPCOption::Talk);
	if (!AvailableQuests.IsEmpty())
	{
		AddDialogueOption(ENPCOption::Quest);
	}
	AddDialogueOption(ENPCOption::Exit);

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
}

void AVMNPC::QuestCompleted()
{
	FVMQuestData QuestData;
	if (CompletedQuests.Dequeue(QuestData))
	{

		EndDialogue();

		GetGameInstance()->GetSubsystem<UVMQuestManager>()->ClearQuest(QuestData);
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

	EndDialogue();

	GetGameInstance()->GetSubsystem<UVMQuestManager>()->AcceptQuest(*QuestData);
}

