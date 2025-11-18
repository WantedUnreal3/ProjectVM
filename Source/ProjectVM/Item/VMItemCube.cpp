// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/VMItemCube.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/InteractComponent.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/ItemFactorySubsystem.h"
#include "Materials/MaterialInterface.h" 
#include "Materials/MaterialInstanceDynamic.h"  
#include "Game/VMRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVMItemCube::AVMItemCube()
{
	PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트로 Box Collision 생성
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Box;
    Box->SetBoxExtent(FVector(50.f, 50.f, 50.f));

    // 자식으로 아이템 큐브 스태틱 매시 생성
    ItemCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemCube"));
    ItemCube->SetupAttachment(RootComponent);
    ItemCube->SetRelativeLocation(FVector(0.0f, 0.0f, -50.f));

    // 기본 메시 설정 (예: 큐브)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshRef(TEXT("/Game/Project/Item/ItemCube.ItemCube"));
    if (CubeMeshRef.Succeeded())
    {
        ItemCube->SetStaticMesh(CubeMeshRef.Object);
    }

    //상호작용 컴포넌트 추가
    InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
    InteractComponent->SetupAttachment(RootComponent);
}

void AVMItemCube::Interact()
{
    UE_LOG(LogTemp, Log, TEXT("Interact Item!!"));
    
    AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    if (PC == nullptr)
    {
        return;
    }

    AVMCharacterHeroBase* Player = Cast< AVMCharacterHeroBase>(PC->GetPawn());

    if (Player==nullptr)
    {
        return;
    }

    Player->GetInventory()->AddNewItem(Equipment, 1);

    Destroy();
}

void AVMItemCube::Setup()
{
    //스태틱 메시에 있는 머티리얼 가져오기
    UMaterialInterface* BaseMat = nullptr;
    if (ItemCube && ItemCube->GetMaterial(0))
    {
        BaseMat = ItemCube->GetMaterial(0);
    }
    if (BaseMat == nullptr)
    {
        return;
    }

    //다이나믹 머터리얼 생성 후 스태틱매시에 적용
    ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);

    if (ItemMaterialInstance == nullptr)
    {
        return;
    }
    ItemCube->SetMaterial(0, ItemMaterialInstance);

    //머터리얼 파라미터 적용
    ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Equipment->GetEquipmentInfo().AtlasCol);
    ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Equipment->GetEquipmentInfo().AtlasRow);
}

// Called when the game starts or when spawned
void AVMItemCube::BeginPlay()
{
	Super::BeginPlay();

    Equipment = GetGameInstance()->GetSubsystem<UItemFactorySubsystem>()->CreateRandomBaseEquipment();

    Setup();
}


