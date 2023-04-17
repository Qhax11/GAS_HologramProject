// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/AttributeSetBase.h"
#include "GameplayEffectExtension.h"


UAttributeSetBase::UAttributeSetBase()
{

}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	// If a Max value changes, adjust current to keep Current % of Current to Max
	if (Attribute == GetMaxHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{

	}
	
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}


// After GameplayEffect which Attributes has changed?
void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

	// If Health is changed
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// For clamping attribute between zero and max value
		//SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

	}
}
