#include "SwdCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Swd/Swd.h"
#include "Swd/Components/AttackComponent.h"
#include "Swd/Components/DamageConsumerComponent.h"
#include "Swd/Components/DamageInflictorComponent.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Components/StaminaComponent.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/Utils/Logger.h"

ASwdCharacter::ASwdCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	InitialMovementSetUp();

	CurrentHealth = MaxHealth;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment Component"));
	DamageConsumerComponent = CreateDefaultSubobject<UDamageConsumerComponent>(TEXT("Damage Consumer Component"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	DamageInflictorComponent = CreateDefaultSubobject<UDamageInflictorComponent>(TEXT("Damage Inflictor Component"));
	LeftLegCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Leg Collosing Box"));
	LeftLegCollisionBox->SetupAttachment(RootComponent);
	LeftLegCollisionBox->SetCollisionProfileName(TEXT("NoCollision"));

	RightLegCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Leg Collosing Box"));
	RightLegCollisionBox->SetupAttachment(RootComponent);
	RightLegCollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
}

void ASwdCharacter::BeginPlay()
{
	Super::BeginPlay();
	const FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		false
	);
	LeftLegCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, BoneSockets::LeftFoodKickSocket);
	RightLegCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, BoneSockets::RightFoodKickSocket);
	RightLegCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASwdCharacter::OnKickOverlapBegin);
	LeftLegCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASwdCharacter::OnKickOverlapBegin);
}

void ASwdCharacter::OnKickOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		DamageInflictorComponent->InflictDamage(
			OtherActor, 10.f * AttackComponent->GetCurrentAttack()->GetAttackDamageFactor()
		);
	}
}


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
	ULogger::Log(ELogLevel::INFO, FString("Actor '" + GetOwner()->GetName() + FString("' is Dead")));
	GetMesh()->SetSimulatePhysics(true);
	GetMovementComponent()->StopActiveMovement();
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetController()->DisableInput(Cast<APlayerController>(GetController()));
	// PlayDeathAnim();


	if (EquipmentComponent->ActualWeaponOnTheHip)
	{
		EquipmentComponent->ActualWeaponOnTheHip->WeaponSkeletalMesh->SetSimulatePhysics("true");
		EquipmentComponent->ActualWeaponOnTheHip->DetachFromActor(FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			false
		));
	}

	if (EquipmentComponent->WeaponInHands)
	{
		EquipmentComponent->WeaponInHands->WeaponSkeletalMesh->SetSimulatePhysics("true");
		EquipmentComponent->WeaponInHands->DetachFromActor(FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			false
		));
	}
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

float ASwdCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ASwdCharacter::UpdateCurrentHealth(float NewValue)
{
	CurrentHealth = NewValue;
}
