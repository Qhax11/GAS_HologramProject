// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterDataAsset.h"
#include "TypeDefinitions.h"

#include "CharacterBase.generated.h"

UCLASS()
class GAS_TEMPLATE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
		UCharacterDataAsset* GetCharacterDataAsset();

	UFUNCTION(BlueprintCallable)
		void SetCharacterDataAsset(FCharacterData CharacterDataInit);

protected:
	virtual void PostInitializeComponents() override;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Instead of TWeakObjectPtrs, you could just have UPROPERTY() hard references or no references at all and just call
	// GetAbilitySystem() and make a GetAttributeSetBase() that can read from the PlayerState or from child classes.
	// Just make sure you test if the pointer is valid before using.
	// I opted for TWeakObjectPtrs because I didn't want a shared hard reference here and I didn't want an extra function call of getting
	// the ASC/AttributeSet from the PlayerState or child classes every time I referenced them in this base class.

	TWeakObjectPtr<class UAbilitySystemComponentBase> AbilitySystemComponent;
	TWeakObjectPtr<class UAttributeSetBase> AttributeSetBase;

	UPROPERTY(EditDefaultsOnly)
		UCharacterDataAsset* CharacterDataAsset;

	UPROPERTY()
		FCharacterData CharacterData;

	void GiveAbilities();
	void ApplyStartupEffects();
	void InitializeAttributes();
	void ApplyPermenantTags();


	//TArray<FGameplayTag>& InOutGameplayTags;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
