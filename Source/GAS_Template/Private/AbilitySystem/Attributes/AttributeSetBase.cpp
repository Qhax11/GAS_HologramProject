// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


UAttributeSetBase::UAttributeSetBase()
{

}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	// If a Max value changes, adjust current to keep Current % of Current to Max
	if (Attribute == GetHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		// For clamping attribute between zero and max value
		//SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	else if (Attribute == GetMovementSpeedAttribute())
	{
		// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}


// After GameplayEffect which Attributes has changed?
void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	FGameplayEffectSpec DamageEffectSpec = Data.EffectSpec;

	// Target actor is who effected
	AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

	// If Health is changed
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{

		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		if (GetHealth() <= 0)
		{
			FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Death"));
			FGameplayEventData Payload;
			Payload.EventTag = DeadTag;
			Payload.Instigator = Instigator;
			Payload.Target = TargetActor;
			Payload.OptionalObject = DamageEffectSpec.Def;
			Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = Data.EvaluatedData.Magnitude;

			FScopedPredictionWindow NewScopedWindow(TargetASC, false);
			TargetASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

	}

	else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
	{

		ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(TargetActor);
		if (!OwnerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("Character cast failed in %s"), *this->GetClass()->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Character cast failed in %s"), *this->GetClass()->GetName()));
			return;
		}
		UCharacterMovementComponent* OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
		if (!OwnerMovementComponent)
		{
			return;
		}
		OwnerMovementComponent->MaxWalkSpeed = GetMovementSpeed();

	}
}
