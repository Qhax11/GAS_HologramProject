// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TypeDefinitions.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
		TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
		FGameplayTagContainer PermenantTags;

};


