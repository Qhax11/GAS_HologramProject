// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"

#include "ActorBase.generated.h"

UCLASS()
class GAS_TEMPLATE_API AActorBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
		class UAbilitySystemComponentBase* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
		class UAttributeSetBase* HardRefAttributeSetBase;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
