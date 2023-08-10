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


	// --------------------------------------
	//	Input
	// --------------------------------------

	// Called when the input is BEING triggered. This is not a one-time press.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Input")
		void OnInputTriggered();

	// Called when the input has released.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Input")
		void OnInputReleased();

	// Called when the confirm input is triggered.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Input")
		void OnInputConfirmTriggered();

	// Called when the cancel input is triggered.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Input")
		void OnInputCancelTriggered();
};
