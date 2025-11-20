// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/VMItemCube.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"

#include "Core/InteractComponent.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/ItemFactorySubsystem.h"
#include "Materials/MaterialInterface.h" 
#include "Materials/MaterialInstanceDynamic.h"  
#include "Game/VMRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Quest/VMQuestManager.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Utils/VMMath.h"

// Sets default values
AVMItemCube::AVMItemCube()
{
	PrimaryActorTick.bCanEverTick = true;

    // 루트 컴포넌트로 Box Collision 생성
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Box;
    Box->SetBoxExtent(FVector(50.f, 50.f, 50.f));

    // 자식으로 아이템 큐브 스태틱 매시 생성
    ItemCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemCube"));
    ItemCube->SetupAttachment(RootComponent);
    ItemCube->SetRelativeLocation(FVector(0.0f, 0.0f, -50.f));
    ItemCube->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

    // 기본 메시 설정 (예: 큐브)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshRef(TEXT("/Game/Project/Item/ItemCube.ItemCube"));
    if (CubeMeshRef.Succeeded())
    {
        ItemCube->SetStaticMesh(CubeMeshRef.Object);
    }

    //상호작용 컴포넌트 추가
    InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
    InteractComponent->SetupAttachment(RootComponent);

    SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
    SplinePath->SetupAttachment(RootComponent);
    SplinePath->bDrawDebug = true;

    Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
    Effect->SetupAttachment(SplinePath);
    Effect->SetAutoActivate(true);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/_SplineVFX/NS/NS_Spline_Wind_Custom.NS_Spline_Wind_Custom"));
    if (NiagaraSystemAsset.Succeeded())
    {
        Effect->SetAsset(NiagaraSystemAsset.Object);
    }

    TimeProgress = 0.0f;

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

    UVMQuestManager* QM = GetGameInstance()->GetSubsystem<UVMQuestManager>();
    if (QM != nullptr)
    {
        QM->NotifyItemCollecting();
    }

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
    CreateEffectPath();
}

void AVMItemCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeProgress += DeltaTime;

    VerticalMove();
    Rotate();
}

void AVMItemCube::CreateEffectPath()
{
    FVector RandomNormalA = FVector(FMath::FRand(), FMath::FRand(), FMath::FRand()).GetSafeNormal();
    FCircle3D CircleA(GetActorLocation(), RandomNormalA, 100.0f);

    FVector RandomNormalB = FVector(FMath::FRand(), FMath::FRand(), FMath::FRand()).GetSafeNormal();
    FCircle3D CircleB(GetActorLocation(), RandomNormalB, 75.0f);

    SplinePath->ClearSplinePoints();

    SplinePath->AddSplinePoint(CircleA.GetLocation(HALF_PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleA.GetLocation(PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleA.GetLocation(PI + HALF_PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleA.GetLocation(TWO_PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleB.GetLocation(HALF_PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleB.GetLocation(PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleB.GetLocation(PI + HALF_PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->AddSplinePoint(CircleB.GetLocation(TWO_PI), ESplineCoordinateSpace::World);
    SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);

    SplinePath->UpdateSpline();
}

void AVMItemCube::VerticalMove()
{
    FVector Offset = FVector::UpVector * FMath::Sin(TimeProgress);
    
    SetActorLocation(GetActorLocation() + Offset);
}

void AVMItemCube::Rotate()
{
    SetActorRotation(GetActorRotation().Clamp().Add(0.f, 0.3f, 0.f));
}


