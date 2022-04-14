#include "EquipWeaponNotifyState.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/EquipmentComponent.h"

void UEquipWeaponNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	if (Character->EquipmentComponent->ActualWeaponOnTheHip)
	{
		Character->EquipmentComponent->AttachWeaponToHand();
	}
	else
	{
		Character->EquipmentComponent->AttachWeaponToThy();
	}
}

void UEquipWeaponNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	if (Character->EquipmentComponent->ActualWeaponOnTheHip)
	{
		Character->EquipmentComponent->ActualWeaponOnTheHip->WeaponState.FinishInteraction();
		Character->ManageCombatState(false);
	}
	else
	{
		Character->EquipmentComponent->WeaponInHands->WeaponState.FinishInteraction();
		Character->ManageCombatState(true);
	}
}
