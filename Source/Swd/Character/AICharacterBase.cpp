#include "AICharacterBase.h"

#include "PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Swd/Components/Modular/Stamina/StaminaComponent.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/Utils/Logger.h"

AAICharacterBase::AAICharacterBase()
{
	SetUpHealthStaminaWidget();
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
	HealthStaminaWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthStaminaWidgetComponent->SetVisibility(false);
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

void AAICharacterBase::UpdateCurrentHealth(float NewValue)
{
	Super::UpdateCurrentHealth(NewValue);
	UpdateHealthOnWidget();
}


void AAICharacterBase::ToggleCombat(const bool Newbool)
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	AnimValues.bIsInCombat = Newbool;
	bUseControllerRotationYaw = Newbool;
	GetCharacterMovement()->bOrientRotationToMovement = !Newbool;
	FName NewSocket = Newbool ? "hand_rSocket" : "spine_03Socket";
	// AttachWeapon(Weapon, NewSocket);
	GetCharacterMovement()->MaxWalkSpeed = (Newbool) ? 187.f : 94.f;
}

//
// void AAICharacterBase::ToggleCrouch(const bool Newbool)
// {
// 	AnimValues.bIsCrouching = Newbool;
// 	const float Speed = AnimValues.bIsInCombat ? 187.f : WalkSpeed;
// 	GetCharacterMovement()->MaxWalkSpeed = (Newbool) ? CrouchedWalkSpeed : Speed;
// }

void AAICharacterBase::ToggleADS(const bool Newbool)
{
	AnimValues.bADS = Newbool;
}

void AAICharacterBase::ToggleSprinting(bool Newbool)
{
	if (Newbool)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = (AnimValues.bIsInCombat) ? 187.f : WalkSpeed;
}
