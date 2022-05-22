#include "SwdCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Swd/Swd.h"
#include "Swd/Animations/Animinstances/AnimInstanceBase.h"
#include "Swd/Components/AttackComponent.h"
#include "Swd/Components/DamageConsumerComponent.h"
#include "Swd/Components/DamageInflictorComponent.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Components/Modular/Stamina/StaminaComponent.h"
#include "Swd/DataAssets/CharacterData.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/Utils/Logger.h"

ASwdCharacter::ASwdCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...

	// set our turn rate for input
	TurnRateGamepad = 50.f;

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
	LeftLegCollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);
	LeftLegCollisionBox->SetRelativeLocation(FVector(18.7f, -19.62f, -86.f));
	LeftLegCollisionBox->SetRelativeRotation(FRotator(0.f, -85.f, 0.f));
	LeftLegCollisionBox->SetRelativeScale3D(FVector(0.1875f, 0.4375f, 0.125f));

	RightLegCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Leg Collosing Box"));
	RightLegCollisionBox->SetupAttachment(RootComponent);
	RightLegCollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);
	RightLegCollisionBox->SetRelativeLocation(FVector(-14.204f, 16.37f, -86.f));
	RightLegCollisionBox->SetRelativeRotation(FRotator(0.f, -40.f, 0.f));
	RightLegCollisionBox->SetRelativeScale3D(FVector(0.1875f, 0.4375f, 0.125f));
}

void ASwdCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CharacterData)
	{
		GetMesh()->SetSkeletalMesh(CharacterData->SkeletalMesh);
		Faction = CharacterData->Faction;
		GetMesh()->SetAnimInstanceClass(CharacterData->AnimInstanceClass);
	}
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

void ASwdCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto AnimInstance = GetAnimInstance();
	if (GetSpeed() == 0)
	{
		if (!RelaxedTimer.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(RelaxedTimer, this, &ASwdCharacter::AtEase,
			                                       1.f, false, FMath::RandRange(
				                                       MinTimeBeforeCanRelax, MaxTimeBeforeCanRelax));
		}
		if (AnimInstance)
		{
			AnimInstance->bHasWeaponEquipped = EquipmentComponent->WeaponInHands;
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RelaxedTimer);
		if (AnimInstance)
		{
			AnimInstance->bIsRelaxed = false;
		}
	}
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

bool ASwdCharacter::IsHostile(ASwdCharacter* Character)
{
	bool IsHostile = Faction != Character->Faction && Character->Faction != EFaction::Neutral;
	return IsHostile;
}

void ASwdCharacter::ManageInput(const bool bShouldEnable)
{
	if (bShouldEnable)
	{
		GetController()->EnableInput(Cast<APlayerController>(GetController()));
		bIsInputEnabled = true;
	}
	else
	{
		GetController()->DisableInput(Cast<APlayerController>(GetController()));
		bIsInputEnabled = false;
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
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void ASwdCharacter::AtEase()
{
	if (auto AnimInstance = GetAnimInstance())
	{
		AnimInstance->bIsRelaxed = true;
	}
}

UAnimInstanceBase* ASwdCharacter::GetAnimInstance()
{
	return Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
}

float ASwdCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

void ASwdCharacter::UpdateCurrentHealth(float NewValue)
{
	CurrentHealth = NewValue;
}

void ASwdCharacter::MakeANoise(FVector Location)
{
	ULogger::Log(ELogLevel::INFO, __FUNCTION__);
	MakeNoise(1.f, nullptr, Location, 0.f, "");
}
