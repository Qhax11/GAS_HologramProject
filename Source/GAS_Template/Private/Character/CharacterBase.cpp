// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/CharacterBase.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/Abilities/GameplayAbilityBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UCharacterDataAsset* ACharacterBase::GetCharacterDataAsset()
{
	return CharacterDataAsset;
}

void ACharacterBase::SetCharacterDataAsset(FCharacterData CharacterDataInit)
{
	CharacterData = CharacterDataInit;
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterDataAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("The character data asset is empty in post"));
		SetCharacterDataAsset(CharacterDataAsset->CharacterData);
		GiveAbilities();
		return;
	}

}



// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!CharacterDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("The character data asset is empty in beginplay"));
		return;
	}

}

void ACharacterBase::GiveAbilities()
{
	if (AbilitySystemComponent.IsValid()) 
	{
		for (TSubclassOf<UGameplayAbilityBase> GameplayAbility : CharacterData.StartupAbilities)
		{
			if (GameplayAbility) 
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GameplayAbility, 1, static_cast<int32>(GameplayAbility.GetDefaultObject()->AbilityInputID), this));
			}
		}
	}
}

void ACharacterBase::ApplyStartupEffects()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : CharacterData.StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
	
}

void ACharacterBase::InitializeAttributes()
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterData.DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}
}

void ACharacterBase::ApplyPermenantTags()
{
	/*
	CharacterData.PermenantTags.GetGameplayTagArray(InOutGameplayTags);

	for (FGameplayTag& InOutGameplayTagsA : InOutGameplayTags)
	{
		AbilitySystemComponent->AddLooseGameplayTag(InOutGameplayTagsA);
	}
	*/

	AbilitySystemComponent->AddLooseGameplayTags(CharacterData.PermenantTags);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

