// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStateBase.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/Attributes/HeroAttributeSetBase.h"


APlayerStateBase::APlayerStateBase()
{

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));

	AttributeSetBase = CreateDefaultSubobject<UHeroAttributeSetBase>(TEXT("HeroAttributeSetBase"));

}

UAbilitySystemComponent* APlayerStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSetBase* APlayerStateBase::GetAttributeSetBase() const
{
	return AttributeSetBase;
}
