#include "AnimInstanceBase.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Utils/Logger.h"

UAnimInstanceBase::UAnimInstanceBase()
{
	bIsInAir = false;
	bIsAnimationBlended = false;
	bIsInCombat = false;
	Speed = 0.f;
	Direction = 0.f;
}

void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PawnOwner = TryGetPawnOwner();
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!PawnOwner) { return; }
	if (ASwdCharacter* Character = Cast<ASwdCharacter>(PawnOwner))
	{
		bIsInAir = Character->GetMovementComponent()->IsFalling();
		Speed = Character->GetSpeed();
		bIsAnimationBlended = Character->bIsAnimationBlended;
		bIsInCombat = Character->GetIsInCombat();
		Direction = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		bIsWeaponSheathed = Character->EquipmentComponent->ActualWeaponOnTheHip;
	}
}
