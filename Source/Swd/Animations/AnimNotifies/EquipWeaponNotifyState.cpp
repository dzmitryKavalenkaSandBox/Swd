#include "EquipWeaponNotifyState.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/EquipmentComponent.h"
#include "Swd/Utils/Logger.h"

void UEquipWeaponNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	if (Character->EquipmentComponent->ActualWeaponOnTheHip)
	{
		Character->EquipmentComponent->AttachWeaponToHand();
		// Character->EquipmentComponent->WeaponOnTheHip->GetDefaultObject()->PlayDrawWeaponSound();
	}
	else
	{
		Character->EquipmentComponent->AttachWeaponToThy();
		// Character->EquipmentComponent->EquippedWeapon->PlaySheathWeaponSound();
	}
}

void UEquipWeaponNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	if (Character->EquipmentComponent->ActualWeaponOnTheHip)
	{
		ULogger::Log(ELogLevel::WARNING, TEXT("FINISHED SHEATHING"));
		Character->EquipmentComponent->ActualWeaponOnTheHip->WeaponState.FinishInteraction();
		Character->ManageCombatState(false);
	}
	else
	{
		ULogger::Log(ELogLevel::WARNING, TEXT("FINISHED EQUIPING"));
		Character->EquipmentComponent->WeaponInHands->WeaponState.FinishInteraction();
		Character->ManageCombatState(true);
	}
}
