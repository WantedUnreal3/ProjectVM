
#include "NPC/VMNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ListView.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameData/VMNPCTalkData.h"
#include "Kismet/GameplayStatics.h"
#include "Core/VMLoadManager.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Dialogue/VMNPCDialogue.h"
#include "UI/VMBillboardComponent.h"
#include "UI/Dialogue/VMDialogueOption.h"
#include "VMNPCEnums.h"

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
	InteractKeyBoxComponent->SetBoxExtent(FVector(45.0f, 45.0f, 100.0f));
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
	static ConstructorHelpers::FClassFinder<UVMNPCDialogue> NPCDialogueRef(TEXT("/Game/Project/UI/WBP_VMNPCDialogue.WBP_VMNPCDialogue_C"));

	if (NPCDialogueRef.Succeeded())
	{
		VMNPCDialogueClass = NPCDialogueRef.Class;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Class"));
	}
}

// Called when the game starts or when spawned
void AVMNPC::BeginPlay()
{
	Super::BeginPlay();

	NPCId = FName(*StaticEnum<ENPCType>()->GetNameStringByValue((int64)NPCType));

	FVMNPCData* LoadedData = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetNPCDataRow(NPCId);
	if (LoadedData)
	{
		NPCData = *LoadedData; // 포인터 → 값 복사
	}

	InteractKeyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVMNPC::OnInteractTriggerOverlapBegin);
	InteractKeyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AVMNPC::OnInteractTriggerOverlapEnd);
	InteractKey->SetVisibility(false);

	//대화 위젯 생성
	if (VMNPCDialogueClass != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("QuestNPC2"));
		VMNPCDialogue = CreateWidget<UVMNPCDialogue>(GetWorld(), VMNPCDialogueClass);
		if (VMNPCDialogue != nullptr)
		{
			//AddDialogueOptions(TEXT("대화하기"), ENPCOption::Talk);
			//AddDialogueOptions(TEXT("퀘스트받기"), ENPCOption::Quest);
			//AddDialogueOptions(TEXT("나가기"), ENPCOption::Exit);

			UE_LOG(LogTemp, Log, TEXT("QuestNPC3"));
			VMNPCDialogue->AddToViewport();
			VMNPCDialogue->SetVisibility(ESlateVisibility::Hidden);

			VMNPCDialogue->SetNPCName(NPCData.GetNPCName());

			FString EnumName = StaticEnum<ENPCType>()->GetNameStringByValue((int64)NPCType);


			//대화 데이터 TArray에 저장
			int32 TalkIndex = 0;
			FVMNPCTalkData* VMNPCTalk;
			do
			{
				FName TalkTag = FName(*FString::Printf(TEXT("%s-%s-%03d"), *NPCId.ToString(), *NPCData.GetStartTalkId().ToString(), ++TalkIndex));
				VMNPCTalk = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetNPCTalkRow(TalkTag);
				if (VMNPCTalk == nullptr)
				{
					UE_LOG(LogTemp, Log, TEXT("Talk Row Not Found: %s"), *TalkTag.ToString());
					break;
				}
				//VMNPCDialogue->AddDialogueText(FText::FromString(VMNPCTalk->Dialogue));

				DialogueTexts.Add(VMNPCTalk);
			} while (!VMNPCTalk->bIsLastLine);
		}
	}
}

void AVMNPC::AddDialogueOptions(FString NewOptionText, ENPCOption NewNPCOption)
{
	if (VMNPCDialogue != nullptr)
	{
		UVMDialogueOption* NewOption = NewObject<UVMDialogueOption>(this);
		NewOption->OptionType = NewNPCOption;
		NewOption->OptionText = NewOptionText;
		NewOption->OwnerNPC = this;

		VMNPCDialogue->DialogueOptionList->AddItem(NewOption);
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
	//if (ATestLab_ProjectVMCharacter* Player = Cast<ATestLab_ProjectVMCharacter>(OtherActor))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Overlapped with Player: %s"), *Player->GetName());
	//	InteractKey->SetVisibility(true);
	//	Player->SetInteractNPC(this);
	//}
}

void AVMNPC::OnInteractTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//만약 NPC가 붙어있어서 여러 NPC가 활성화 된 상태라면 하나만 선택된다. 바로 옆으로 이동하면 nullptr로 설정하기 때문에 다른 NPC가 실행되지는 않는다.
	//일단 NPC를 붙어서 만들지는 않을 예정이르모 이렇게 처리한다.
	//if (ATestLab_ProjectVMCharacter* Player = Cast<ATestLab_ProjectVMCharacter>(OtherActor))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Exit with Player: %s"), *Player->GetName());
	//	InteractKey->SetVisibility(false);
	//	Player->SetInteractNPC(nullptr);
	//}
}

void AVMNPC::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("Interact Quest"));
	if (VMNPCDialogue != nullptr)
	{
		VMNPCDialogue->SetVisibility(ESlateVisibility::Visible);
		NextDialogue();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Quest Error"));
	}
}

bool AVMNPC::NextDialogue()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (CurrentDialogueIndex < DialogueTexts.Num())
	{
		VMNPCDialogue->SetNPCText(FText::FromString(DialogueTexts[CurrentDialogueIndex]->Dialogue));
		CurrentDialogueIndex++;

		if (DialogueTexts.Num() == CurrentDialogueIndex)
		{
			UE_LOG(LogTemp, Display, TEXT("Now"));
			VMNPCDialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Visible);

			if (PC != nullptr)
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(VMNPCDialogue->DialogueOptionList->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}

		return true;
	}
	else
	{
		// All dialogues have been played
		VMNPCDialogue->SetVisibility(ESlateVisibility::Hidden);
		CurrentDialogueIndex = 0; // Reset for next time
		VMNPCDialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Hidden);
		if (PC != nullptr)
		{
			PC->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
		return false;
	}
}

void AVMNPC::StartQuest()
{
	UE_LOG(LogTemp, Log, TEXT("Start Quest"));
}

void AVMNPC::StartDailyTalk()
{
	UE_LOG(LogTemp, Log, TEXT("Start Daily Talk"));
}

void AVMNPC::EndDialogue()
{
	UE_LOG(LogTemp, Log, TEXT("End Dialogue"));
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	VMNPCDialogue->SetVisibility(ESlateVisibility::Hidden);
	CurrentDialogueIndex = 0; // Reset for next time
	VMNPCDialogue->DialogueOptionList->SetVisibility(ESlateVisibility::Hidden);
	if (PC != nullptr)
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);

		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (PlayerCharacter != nullptr)
		{
			//ATestLab_ProjectVMCharacter* TestLabCharacter = Cast<ATestLab_ProjectVMCharacter>(PlayerCharacter);
			//if (TestLabCharacter != nullptr)
			//{
			//	TestLabCharacter->ChangeInputMode(EInputMode::Default);
			//}
		}
	}
}

