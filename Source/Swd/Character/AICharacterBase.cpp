#include "AICharacterBase.h"

#include "PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Components/Modular/Stamina/StaminaComponent.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/Utils/Logger.h"

AAICharacterBase::AAICharacterBase()
{
	InitialMovementSetUp();
	SetUpHealthStaminaWidget();
	SetUpDetectorWidget();
	SphereAroundAI = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Around AI"));
	// to make turning smoother
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	SphereAroundAI->SetupAttachment(RootComponent);
	SphereAroundAI->SetCollisionProfileName(FName("OverlapAll"));
	SphereAroundAI->SetGenerateOverlapEvents(true);
	SphereAroundAI->SetSphereRadius(600.f);
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UpdateHealthOnWidget();
	UpdateStaminaOnWidget();
	SphereAroundAI->OnComponentBeginOverlap.AddDynamic(this, &AAICharacterBase::OnSphereOverlapBegin);
	SphereAroundAI->OnComponentEndOverlap.AddDynamic(this, &AAICharacterBase::OnSphereOverlapEnd);

	UpdateWidgetRef();
}

void AAICharacterBase::SetUpHealthStaminaWidget()
{
	HealthStaminaWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Stamina Widget Component"));
	HealthStaminaWidgetComponent->SetupAttachment(RootComponent);
	HealthStaminaWidgetComponent->SetDrawSize(FVector2D(80, 20));
	HealthStaminaWidgetComponent->SetRelativeLocation(FVector(0, 0, 110.f));
	HealthStaminaWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthStaminaWidgetComponent->SetVisibility(false);
}

void AAICharacterBase::SetUpDetectorWidget()
{
	DetectorWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Detector Widget Component"));
	DetectorWidgetComponent->SetupAttachment(RootComponent);
	DetectorWidgetComponent->SetDrawSize(FVector2D(80, 10));
	DetectorWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DetectorWidgetComponent->SetRelativeLocation(FVector(0, 0, 120.f));
}

void AAICharacterBase::UpdateHealthOnWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->HealthBar->SetPercent(GetCurrentHealth() / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}

void AAICharacterBase::UpdateStaminaOnWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->StaminaBar->SetPercent(StaminaComponent->CurrentStamina / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}

bool AAICharacterBase::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation,
                                     int32& NumberOfLoSChecksPerformed, float& OutSightStrength,
                                     const AActor* IgnoreActor, const bool* bWasVisible,
                                     int32* UserData) const
{
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));

	FHitResult HitResult;
	FVector SocketLocation = GetMesh()->GetSocketLocation(PerceptionTarget);

	const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SocketLocation,
	                                                                FCollisionObjectQueryParams(ECC_TO_BITFIELD
		                                                                (ECC_WorldStatic) | ECC_TO_BITFIELD(
			                                                                ECC_WorldDynamic)),
	                                                                FCollisionQueryParams(NAME_AILineOfSight, true,
		                                                                IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHitSocket == false || (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = SocketLocation;
		OutSightStrength = 1;

		return true;
	}

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation(),
	                                                          FCollisionObjectQueryParams(
		                                                          ECC_TO_BITFIELD(ECC_WorldStatic) |
		                                                          ECC_TO_BITFIELD(ECC_WorldDynamic)),
	                                                          FCollisionQueryParams
	                                                          (NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHit == false || (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		return true;
	}

	OutSightStrength = 0;
	return false;
}

void AAICharacterBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		HealthStaminaWidgetComponent->SetVisibility(true);
	}
}

void AAICharacterBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		HealthStaminaWidgetComponent->SetVisibility(false);
	}
}

void AAICharacterBase::ManageCombatState(bool bEnableCombat)
{
	Super::ManageCombatState(bEnableCombat);
	PostCombatStateEnabled();
}

void AAICharacterBase::InitialMovementSetUp()
{
	Super::InitialMovementSetUp();
	ULogger::Log(ELogLevel::WARNING, TEXT("Setting movement from Ai"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AAICharacterBase::UpdateCurrentHealth(float NewValue)
{
	Super::UpdateCurrentHealth(NewValue);
	UpdateHealthOnWidget();
}


void AAICharacterBase::ToggleCombat(const bool ShouldEnableCombat)
{
	// GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	ManageCombatState(ShouldEnableCombat);
	AnimValues.bIsInCombat = ShouldEnableCombat;
	// AnimValues.bIsInCombat = ShouldEnableCombat;
	// bUseControllerRotationYaw = ShouldEnableCombat;
	// GetCharacterMovement()->bOrientRotationToMovement = !ShouldEnableCombat;
	// FName NewSocket = ShouldEnableCombat ? "hand_rSocket" : "spine_03Socket";
	// // AttachWeapon(Weapon, NewSocket);
	// GetCharacterMovement()->MaxWalkSpeed = (ShouldEnableCombat) ? 187.f : 94.f;
}

void AAICharacterBase::ToggleArmedState(const bool ShouldArmSelf)
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	if (ShouldArmSelf && EquipmentComponent->ActualWeaponOnTheHip)
	{
		EquipmentComponent->EquipWeapon();
	}
	if (!ShouldArmSelf && EquipmentComponent->WeaponInHands)
	{
		EquipmentComponent->SheathWeapon();
	}
}

void AAICharacterBase::ToggleADS(const bool Newbool)
{
	AnimValues.bADS = Newbool;
}

void AAICharacterBase::ToggleSprinting(bool bShouldSprint)
{
	if (bShouldSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}
