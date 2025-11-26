// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VMHeroSkillComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSkillExecuteHandler, class USkillBase* /* Skill */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMHeroSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVMHeroSkillComponent();

	FSkillExecuteHandler OnBasicSkillExecute;
	FSkillExecuteHandler OnAdvancedSkillExecute;
	FSkillExecuteHandler OnMovementSkillExecute;
	FSkillExecuteHandler OnUltimateSkillExecute;

	void ExecuteBasicSkill(class AVMCharacterHeroBase* Owner, class UVMHeroStatComponent* StatComp);
	void ExecuteAdvancedSkill(class AVMCharacterHeroBase* Owner, class UVMHeroStatComponent* StatComp);
	void ExecuteMovementSkill(class AVMCharacterHeroBase* Owner, class UVMHeroStatComponent* StatComp);
	void ExecuteUltimateSkill(class AVMCharacterHeroBase* Owner, class UVMHeroStatComponent* StatComp);

	void BindBasicSkill(class USkillBase* InSkill);
	void BindAdvancedSkill(class USkillBase* InSkill);
	void BindMovementSkill(class USkillBase* InSkill);
	void BindUltimateSkill(class USkillBase* InSkill);
	
	void BindBasicSkillMontage(class UAnimMontage* InMontage);
	void BindAdvancedSkillMontage(class UAnimMontage* InMontage);
	void BindMovementSkillMontage(class UAnimMontage* InMontage);
	void BindUltimateSkillMontage(class UAnimMontage* InMontage);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BasicSkillMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AdvancedSkillMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MovementSkillMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> UltimateSkillMontage;
};
