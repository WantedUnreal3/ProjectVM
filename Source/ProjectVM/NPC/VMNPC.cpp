
#include "NPC/VMNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ListView.h"
#include "Components/WidgetComponent.h"
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
		DialogueTexts.Add(VMNPCTalk);
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
		Dialogue->SetNPCText(FText::FromString(DialogueTexts[CurrentDialogueIndex]->Dialogue));
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

void AVMNPC::EnterShop()
{
	//자식에서 구현
}
