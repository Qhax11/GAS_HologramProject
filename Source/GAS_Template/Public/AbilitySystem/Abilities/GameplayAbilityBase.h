// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS_Template/GAS_Template.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGameplayAbilityBase();


	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EAbilityInputID AbilityInputID = EAbilityInputID::None;

	int x;

};
