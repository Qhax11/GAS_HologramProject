// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalculations/PhysicalDamageExecCalculation.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/Attributes/AttributeSetBase.h"



void UPhysicalDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	const UAttributeSetBase* TargetAttributes = TargetAbilitySystemComponent->GetSet<UAttributeSetBase>();
	const UAttributeSetBase* SourceAttributes = SourceAbilitySystemComponent->GetSet<UAttributeSetBase>();

	// Gameplay tags that are attached to the ***Effect*** (not the actor!)
	FGameplayTagContainer effectAssetTags;
	Spec.GetAllAssetTags(effectAssetTags);

#pragma region CapturedAttribute

	// Source attributes
	float Damage = SourceAttributes->GetPhysicalDamage();

	// If there is bonus damage in effect we captured it
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	// Target attributes
	float PhysicalDamageReduction = TargetAttributes->GetArmor();
	float TargetHealth = TargetAttributes->GetHealth();

	

	float MitigatedDamage = Damage - PhysicalDamageReduction;

	// This clamp prevents us from doing more damage than there is health available.
	float DamageDone = FMath::Clamp(MitigatedDamage, 0.0f, TargetHealth);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(TargetAttributes->GetHealthAttribute(), EGameplayModOp::Additive, -DamageDone));


#pragma endregion
}
