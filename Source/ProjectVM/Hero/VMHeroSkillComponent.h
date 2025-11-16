// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VMHeroSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMHeroSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVMHeroSkillComponent();

	void ExecuteBasicSkill(class AVMCharacterHeroBase* Owner, class UVMHeroStatComponent* StatComp);
	void ExecuteAdvancedSkill(class UVMHeroStatComponent* StatComp);
	void ExecuteMovementSkill(class UVMHeroStatComponent* StatComp);
	void ExecuteUltimateSkill(class UVMHeroStatComponent* StatComp);

	void BindBasicSkill(class USkillBase* InSkill);
	void BindAdvancedSkill(class USkillBase* InSkill);
	void BindMovementSkill(class USkillBase* InSkill);
	void BindUltimateSkill(class USkillBase* InSkill);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkillBase> BasicSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkillBase> AdvancedSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkillBase> MovementSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkillBase> UltimateSkill;
};
