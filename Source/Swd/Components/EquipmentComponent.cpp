#include "EquipmentComponent.h"

#include "Swd/Swd.h"
#include "Swd/Character/SwdCharacter.h"
#include "Engine/DataTable.h"
#include "Swd/Utils/Logger.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetUpAnimations();
}

bool UEquipmentComponent::CanEquipWeapon()
{
	return WeaponOnTheHipSlot && WeaponOnTheHipSlot.GetDefaultObject()->WeaponState.CanActOnWeapon();
}

void UEquipmentComponent::EquipWeapon()
{
	if (CanEquipWeapon())
	{
		WeaponOnTheHipSlot.GetDefaultObject()->WeaponState.StartInteraction();
		PlayDataTableAnimation(GetOwnerCharacter(), EquipSwordAnimMontageDataTable, false, AnimAction::Equip);
	}
}

void UEquipmentComponent::AttachWeaponToHand()
{
	if (ActualWeaponOnTheHip)
	{
		// AWeaponBase* Weapon = Cast<AWeaponBase>(WeaponOnTheHip->GetDefaultObject());
		ULogger::Log(ELogLevel::WARNING, __FUNCTION__);
		ActualWeaponOnTheHip->AttachToComponent(
			GetOwnerCharacter()->GetMesh(),
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true
			),
			BoneSockets::WeaponHandle
		);
		ActualWeaponOnTheHip = nullptr;
	}
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponOnTheHipSlot)
	{
		ActualWeaponOnTheHip = GetWorld()->SpawnActor<AWeaponBase>(WeaponOnTheHipSlot);
		ActualWeaponOnTheHip->WeaponSkeletalMesh->SetSimulatePhysics(false);
		ActualWeaponOnTheHip->AttachToComponent(
			GetOwnerCharacter()->GetMesh(),
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true
			),
			BoneSockets::HipWeaponSocket
		);
	}
}

void UEquipmentComponent::SetUpAnimations()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> EquipWeaponDataObject(
		AM_DataTableReference::EquipSheathWeapon);
	if (EquipWeaponDataObject.Succeeded())
	{
		EquipSwordAnimMontageDataTable = EquipWeaponDataObject.Object;
	}
}

ASwdCharacter* UEquipmentComponent::GetOwnerCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}
