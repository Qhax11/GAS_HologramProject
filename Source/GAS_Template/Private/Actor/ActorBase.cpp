// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorBase.h"
#include "AbilitySystem/Attributes/AttributeSetBase.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"

// Sets default values
AActorBase::AActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HardRefAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));

	// Set our parent's TWeakObjectPtr
	//AbilitySystemComponent = HardRefAbilitySystemComponent;

	// Create the attribute set
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	HardRefAttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set our parent's TWeakObjectPtr
	//AttributeSetBase = HardRefAttributeSetBase;
}

// Called when the game starts or when spawned
void AActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


UAbilitySystemComponent* AActorBase::GetAbilitySystemComponent() const
{
	return HardRefAbilitySystemComponent;
}