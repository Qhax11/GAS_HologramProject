// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalculationBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API UExecCalculationBase : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("EffectData.Damage"));
};
