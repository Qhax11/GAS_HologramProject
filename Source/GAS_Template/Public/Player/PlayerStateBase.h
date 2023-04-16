// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API APlayerStateBase : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	

public:
	APlayerStateBase();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UAttributeSetBase* GetAttributeSetBase() const;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
		class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
		class UAttributeSetBase* AttributeSetBase;
};
