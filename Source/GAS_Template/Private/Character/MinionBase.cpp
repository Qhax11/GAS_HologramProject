// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MinionBase.h"
#include "AbilitySystem/Attributes/AttributeSetBase.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"

AMinionBase::AMinionBase()
{

	HardRefAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));

	// Set our parent's TWeakObjectPtr
	AbilitySystemComponent = HardRefAbilitySystemComponent;

	// Create the attribute set
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	HardRefAttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set our parent's TWeakObjectPtr
	AttributeSetBase = HardRefAttributeSetBase;

}

void AMinionBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		GiveAbilities();
		InitializeAttributes();
		ApplyStartupEffects();
		ApplyPermenantTags();
	}
}
