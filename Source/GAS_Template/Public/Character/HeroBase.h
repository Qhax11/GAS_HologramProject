// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "InputAction.h"

#include "HeroBase.generated.h"

/**
 *   Stores an InputAction with an Ability to trigger.
 *\n Make sure that the AbilityToTrigger is already granted on the hero when using this AbilityInputPair.
**/
USTRUCT(BlueprintType)
struct FAbilityInputPair
{
	GENERATED_USTRUCT_BODY()

public:
	/** Description of the AbilityInputPair. This makes it more readible once used in arrays. */
	UPROPERTY(EditDefaultsOnly)
		FName Description;

	// This Input Action must trigger in order to trigger the ability.
	UPROPERTY(EditDefaultsOnly, meta = (DisplayThumbnail = "false"))
		UInputAction* InputAction;

	// This Input Action must not be triggered in order to trigger the ability.
	UPROPERTY(EditDefaultsOnly, meta = (DisplayThumbnail = "false"))
		TObjectPtr<const UInputAction> IgnoreInputAction;

	// Specify the ability to trigger. Ability must be granted to the Hero.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameplayAbility> AbilityToTrigger;
};

/**
 *   Stores NativeInputActions (such as Move, Look) and AbilityInputActions (any ability can be bound to any input).
 *\n
 *\n Any entry in the NativeInputActions will be applied automatically to the hero.
 *\n Any entry in the AbilityInputPairArray will be applied automatically to the hero.
 *\n Any entry in the AbilityInputPairArray will get their specified InputAction and GameplayAbility bound to each other.
 *\n
 *\n Feel free to expand NativeInputActions by adding more of them to the class header and assign them
 *   on blueprints that derives from this class.
*/
UCLASS(Blueprintable)
class UHeroInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NativeInputActions")
		UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NativeInputActions")
		UInputAction* InputLookMouse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NativeInputActions")
		UInputAction* InputLookStick;

	// Is used to confirm targeting abilities.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NativeInputActions")
		UInputAction* InputConfirm;

	// Is used to cancel targeting abilities.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NativeInputActions")
		UInputAction* InputCancel;

	// Binds each entry's InputAction to their specified GameplayAbility at runtime.
	// *** Make sure that the chosen GameplayAbility is granted to the hero.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInputActions", Meta = (TitleProperty = "Description"))
		TArray<FAbilityInputPair> AbilityInputPairArray;
};

/**
 * 
 */
UCLASS()
class GAS_TEMPLATE_API AHeroBase : public ACharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

public:
	AHeroBase(const class FObjectInitializer& ObjectInitializer);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void PossessedBy(AController* NewController) override;

	// --------------------
	//        Input
	// --------------------

	/**
	 *   HeroInputConfig stores and applies all Native Input Actions and Ability Input Actions.
	 *\n This has to be set on each different hero separately.
	 *
	 *\n For example, Native Input Actions can be Move or even Looking around with Mouse or Joystick.
	 *\n For example, Ability Input Actions can be Jumping or using Ability1 (default keybind: LMB) and many more!
	**/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		TSubclassOf<UHeroInputConfig> HeroInputConfig;

	bool bASCInputBound = false;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Confirm(const FInputActionValue& InputActionValue);
	void Input_Cancel(const FInputActionValue& InputActionValue);

	// ASC Abilities
	void OnInputActionTriggered(const FInputActionValue& Value, FAbilityInputPair AbilityInputPair);
	void OnInputActionCompleted(const FInputActionValue& Value, FAbilityInputPair AbilityInputPair);

	// Called from SetupPlayerInputComponent. Is used to bind native input such as moving or looking around.
	// In order for this to work, you need to set the HeroInputConfig variable.
	void BindNativeInput();

	/**
	*   Binds abilities to their associated inputs.
	*
	*\n Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	*   call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	*
	*\n Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	*
	*\n In order for this to work, you need to set the HeroInputConfig variable.
	*
	*\n @see UHeroInputConfig
	**/
	void BindASCInput();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
