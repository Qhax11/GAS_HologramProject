// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ExecCalculations/ExecCalculationBase.h"
#include "PhysicalDamageExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API UPhysicalDamageExecCalculation : public UExecCalculationBase
{
	GENERATED_BODY()
	
public:


	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
