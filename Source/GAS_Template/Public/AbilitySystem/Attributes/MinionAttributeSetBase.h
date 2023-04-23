// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/AttributeSetBase.h"
#include "MinionAttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API UMinionAttributeSetBase : public UAttributeSetBase
{
	GENERATED_BODY()
public:

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	UPROPERTY(BlueprintReadOnly, Category = "Endurance")
		FGameplayAttributeData MinionAttribute;
	ATTRIBUTE_ACCESSORS(UMinionAttributeSetBase, MinionAttribute)
	
};
