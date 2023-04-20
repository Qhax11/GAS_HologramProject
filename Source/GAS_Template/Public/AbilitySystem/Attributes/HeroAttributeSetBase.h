// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/AttributeSetBase.h"
#include "HeroAttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API UHeroAttributeSetBase : public UAttributeSetBase
{
	GENERATED_BODY()

public:

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	
	UPROPERTY(BlueprintReadOnly, Category = "Endurance")
		FGameplayAttributeData HeroAttribute;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSetBase, HeroAttribute)



};
