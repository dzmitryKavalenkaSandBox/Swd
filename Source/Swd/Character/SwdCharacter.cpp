#include "SwdCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Components/HealthComponent.h"
#include "Swd/Utils/Logger.h"

ASwdCharacter::ASwdCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	InitialMovementSetUp();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
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
	}
	else
	{
		EquipmentComponent->SheathWeapon();
	}
}

void ASwdCharacter::ManageCombatState(bool bEnableCombat)
{
	bIsInCombat = bEnableCombat;
	if (bEnableCombat)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeedCombat;
		if (!GetCharacterMovement()->bUseControllerDesiredRotation)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
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

void ASwdCharacter::HandleDeathBehavior()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMovementComponent()->StopActiveMovement();
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetController()->DisableInput(Cast<APlayerController>(GetController()));
	// PlayDeathAnim();
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

void ASwdCharacter::InitialMovementSetUp()
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
}
