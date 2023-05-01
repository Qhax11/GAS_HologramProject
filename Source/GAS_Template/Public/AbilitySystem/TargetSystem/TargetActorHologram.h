// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Curves/CurveFloat.h"
#include "TargetActorHologram.generated.h"
/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API ATargetActorHologram : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ATargetActorHologram();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "Hologram")
		class UCurveFloat* LongOfActorCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "Hologram")
		class APawn* InstigatorPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "Hologram")
		float HeightOfTrace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hologram")
		float LongOfTrace;


	APawn* MasterPawn;
	
};
