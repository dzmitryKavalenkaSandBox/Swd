#include "SwdCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Utils/Logger.h"

//////////////////////////////////////////////////////////////////////////
// ASwdCharacter

ASwdCharacter::ASwdCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment Component"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASwdCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
}

void ASwdCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASwdCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

float ASwdCharacter::GetSpeed()
{
	return GetVelocity().Size();
}

void ASwdCharacter::EquipSheathWeapon()
{
	if (EquipmentComponent->ActualWeaponOnTheHip)
	{
		EquipmentComponent->EquipWeapon();
		ManageCombatState(true);
	}
	else
	{
		EquipmentComponent->SheathWeapon();
		ManageCombatState(false);
	}
}

void ASwdCharacter::ManageCombatState(bool bEnableCombat)
{
	bIsInCombat = bEnableCombat;
	if (bEnableCombat)
	{
		if (!GetCharacterMovement()->bUseControllerDesiredRotation)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else
	{
		if (GetCharacterMovement()->bUseControllerDesiredRotation)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

bool ASwdCharacter::GetIsInCombat() const
{
	return bIsInCombat;
}

void ASwdCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASwdCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
