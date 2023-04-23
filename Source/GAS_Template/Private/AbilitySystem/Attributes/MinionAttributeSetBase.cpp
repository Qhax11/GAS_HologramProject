// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/MinionAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Character/MinionBase.h"


void UMinionAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UMinionAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	FGameplayEffectSpec DamageEffectSpec = Data.EffectSpec;


	AMinionBase* Minion = Cast<AMinionBase>(Data.Target.AbilityActorInfo->AvatarActor.Get());


	// If Health is changed
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// For clamping attribute between zero and max value
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));


		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		if (GetHealth() <= 0)
		{
			FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Death"));
			FGameplayEventData Payload;
			Payload.EventTag = DeadTag;
			Payload.Instigator = Instigator;
			Payload.Target = Minion;
			Payload.OptionalObject = DamageEffectSpec.Def;
			Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = Data.EvaluatedData.Magnitude;

			FScopedPredictionWindow NewScopedWindow(Minion->GetAbilitySystemComponent(), true);
			Minion->GetAbilitySystemComponent()->HandleGameplayEvent(Payload.EventTag, &Payload);


		}

	}
}
