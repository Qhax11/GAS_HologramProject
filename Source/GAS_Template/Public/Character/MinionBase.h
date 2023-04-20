// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "MinionBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API AMinionBase : public ACharacterBase
{
	GENERATED_BODY()

public:

	AMinionBase();
	

protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	    class UAbilitySystemComponentBase* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
		class UAttributeSetBase* HardRefAttributeSetBase;

	virtual void BeginPlay() override;

};
