// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/HeroAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "Character/HeroBase.h"


void UHeroAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{


}

void UHeroAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

	Super::PostGameplayEffectExecute(Data);

	

}
