// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroBase.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GAS_Template/GAS_Template.h"
#include "Player/PlayerStateBase.h"
#include "Player/PlayerControllerBase.h"
#include "AbilitySystem/Abilities/GameplayAbilityBase.h"


//////////////////////////////////////////////////////////////////////////
// AHeroBase

AHeroBase::AHeroBase(const class FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


}

void AHeroBase::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHeroBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerStateBase* PS = GetPlayerState<APlayerStateBase>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UAbilitySystemComponentBase>(PS->GetAbilitySystemComponent()); 

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		GiveAbilities();
		InitializeAttributes();
		ApplyStartupEffects();
		ApplyPermenantTags();

	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHeroBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Check if Enhanced Input Component is valid.
	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component is not valid! Check GDHeroCharacter.cpp"));
		return;
	}

	// Check if the designers added a valid HeroInputConfig into this GDHeroCharacter's derived class blueprint.
	if (!HeroInputConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("GDHeroCharacter doesn't have a valid HeroInputConfig set in their class defaults. No Input will be bound!"));
		return;
	}

	// Bind Native Input, such as moving or looking around.
	BindNativeInput();

	// Bind Hero Abilities to Input Actions. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();

}

void AHeroBase::BindASCInput()
{
	if (!bASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);
		check(EIC);

		// For each AbilityInputPair defined in the Config, bind InputActions with their associated Abilities.
		for (FAbilityInputPair& AbilityInputPair : HeroInputConfig.GetDefaultObject()->AbilityInputPairArray)
		{
			UInputAction* InputAction = AbilityInputPair.InputAction;
			TSubclassOf<UGameplayAbility>& AbilityToTrigger = AbilityInputPair.AbilityToTrigger;

			// Invalid data checks
			if (!AbilityToTrigger || !InputAction)
			{
				UE_LOG(LogTemp, Error, TEXT("One of the AbilityInputPairs has invalid data! "
					"AbilityToTrigger and/or InputAction is null! Check this Hero's HeroInputConfig variable: %s"), *GetName());
				continue;
			}

			// Bind all InputAction Triggers.
			EIC->BindAction(InputAction, ETriggerEvent::Triggered, this, &AHeroBase::OnInputActionTriggered, AbilityInputPair);
			// Bind all InputAction Completions.
			EIC->BindAction(InputAction, ETriggerEvent::Completed, this, &AHeroBase::OnInputActionCompleted, AbilityInputPair);
		}

		bASCInputBound = true;
	}
}

void AHeroBase::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Value.Y);
	}
}

void AHeroBase::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

void AHeroBase::Input_LookStick(const FInputActionValue& InputActionValue)
{
}

void AHeroBase::Input_Confirm(const FInputActionValue& InputActionValue)
{
	auto ASC = GetAbilitySystemComponent();
	if (!ASC) return;

	// Find AbilityCDO's Spec.
	for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.IsActive()) 
		{
			UGameplayAbility* AbilityCDO = Spec.Ability;

			// If the Ability is set to be non-instanced, then call the custom OnInputReleased function on AbilityCDO.
			TArray<UGameplayAbility*> Abilities{ AbilityCDO };

			// If the ability is instanced, Reset the Abilities Array and put all the instances into it.
			if (AbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				Abilities.Reset();
				Abilities = Spec.GetAbilityInstances();
			}

			// For each Instance of this ability (only AbilityCDO if the ability is NonInstanced), call the custom OnInputTriggered function.
			for (UGameplayAbility* Instance : Abilities)
			{
				if (UGameplayAbilityBase* GDAbility = Cast<UGameplayAbilityBase>(Instance))
				{
					GDAbility->OnInputConfirmTriggered();
				}
			}
		}
	}
}

void AHeroBase::Input_Cancel(const FInputActionValue& InputActionValue)
{
}

void AHeroBase::OnInputActionTriggered(const FInputActionValue& Value, FAbilityInputPair AbilityInputPair)
{
	auto ASC = GetAbilitySystemComponent();
	if (!ASC) return;

	// If IgnoreInputAction exists then we should check whether that InputAction is triggered, if so we shouldn't activate any abilities.
	if (AbilityInputPair.IgnoreInputAction)
	{
		// Get Player Controller
		if (APlayerControllerBase* PC = Cast<APlayerControllerBase>(GetController()))
		{
			// Find the InputActionInstance and check if it's triggered.
			if (UEnhancedPlayerInput* EPI = Cast<UEnhancedPlayerInput>(PC->PlayerInput)) {
				const FInputActionInstance* IgnoreInputActionInstance = EPI->FindActionInstanceData(AbilityInputPair.IgnoreInputAction);
				if (IgnoreInputActionInstance && IgnoreInputActionInstance->GetTriggerEvent() == ETriggerEvent::Triggered)
				{
					return;
				}
			}
		}
	}

	// Get the AbilityCDO (Class Default Object) from AbilityInputPair
	UGameplayAbility* AbilityCDO = AbilityInputPair.AbilityToTrigger->GetDefaultObject<UGameplayAbility>();
	FGameplayAbilitySpec* AbilitySpec = nullptr;

	// Find AbilityCDO's Spec.
	for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (AbilityCDO == Spec.Ability.Get())
		{
			AbilitySpec = &Spec;
			break;
		}
	}

	// Ability Spec couldn't be found because it is not granted to hero. The InputConfig given to the hero is faulty.
	if (!AbilitySpec)
	{
		check(AbilitySpec);
		UE_LOG(LogTemp, Error, TEXT("Specified Ability is not granted to hero! %s"), *AbilityCDO->GetName());
		return;
	}

	// Try to activate the associated Gameplay Ability with the given InputMap's InputAction.
	ASC->TryActivateAbility(AbilitySpec->Handle);

	// If the Ability is set to be non-instanced, then call the custom OnInputReleased function on AbilityCDO.
	TArray<UGameplayAbility*> Abilities{ AbilityCDO };

	// If the ability is instanced, Reset the Abilities Array and put all the instances into it.
	if (AbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Abilities.Reset();
		Abilities = AbilitySpec->GetAbilityInstances();
	}

	// For each Instance of this ability (only AbilityCDO if the ability is NonInstanced), call the custom OnInputTriggered function.
	for (UGameplayAbility* Instance : Abilities)
	{
		if (UGameplayAbilityBase* GDAbility = Cast<UGameplayAbilityBase>(Instance))
		{
			GDAbility->OnInputTriggered();
		}
	}

	// Call the native InputPressed function on AbilitySpec.
	ASC->AbilitySpecInputPressed(*AbilitySpec);
}

void AHeroBase::OnInputActionCompleted(const FInputActionValue& Value, FAbilityInputPair AbilityInputPair)
{
	if (auto ASC = GetAbilitySystemComponent())
	{
		// Get the AbilityCDO (Class Default Object) from AbilityInputPair
		UGameplayAbility* AbilityCDO = AbilityInputPair.AbilityToTrigger->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec* AbilitySpec = nullptr;

		// Find AbilityCDO's Spec.
		for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
		{
			if (AbilityCDO == Spec.Ability.Get())
			{
				AbilitySpec = &Spec;
				break;
			}
		}

		// Ability Spec couldn't be found because it is not granted to hero. The InputConfig given to the hero is faulty.
		if (!AbilitySpec)
		{
			check(AbilitySpec);
			UE_LOG(LogTemp, Error, TEXT("Specified Ability is not granted to hero! %s"), *AbilityCDO->GetName());
			return;
		}

		// If the Ability is set to be non-instanced, then call the custom OnInputReleased function on AbilityCDO.
		TArray<UGameplayAbility*> Abilities{ AbilityCDO };

		// If the ability is instanced, Reset the Abilities Array and put all the instances into it.
		if (AbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
		{
			Abilities.Reset();
			Abilities = AbilitySpec->GetAbilityInstances();
		}

		// For each Instance of this ability (only AbilityCDO if the ability is NonInstanced), call the custom OnInputReleased function.
		for (UGameplayAbility* Instance : Abilities)
		{
			if (UGameplayAbilityBase* Ability = Cast<UGameplayAbilityBase>(Instance))
			{
				Ability->OnInputReleased();
			}
		}

		// Call the native InputReleased function on AbilitySpec.
		ASC->AbilitySpecInputReleased(*AbilitySpec);
	}
}

void AHeroBase::BindNativeInput()
{
	if (auto EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(HeroInputConfig.GetDefaultObject()->InputMove, ETriggerEvent::Triggered, this, &AHeroBase::Input_Move);
		EIC->BindAction(HeroInputConfig.GetDefaultObject()->InputLookMouse, ETriggerEvent::Triggered, this, &AHeroBase::Input_LookMouse);
		EIC->BindAction(HeroInputConfig.GetDefaultObject()->InputLookStick, ETriggerEvent::Triggered, this, &AHeroBase::Input_LookStick);
		EIC->BindAction(HeroInputConfig.GetDefaultObject()->InputConfirm, ETriggerEvent::Triggered, this, &AHeroBase::Input_Confirm);
		EIC->BindAction(HeroInputConfig.GetDefaultObject()->InputCancel, ETriggerEvent::Triggered, this, &AHeroBase::Input_Cancel);
	}
}
