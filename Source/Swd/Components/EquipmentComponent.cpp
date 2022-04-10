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
	return ActualWeaponOnTheHip && ActualWeaponOnTheHip->WeaponState.CanActOnWeapon();
}

bool UEquipmentComponent::CanSheathWeapon()
{
	return WeaponInHands && WeaponInHands->WeaponState.CanActOnWeapon();
}

void UEquipmentComponent::EquipWeapon()
{
	if (CanEquipWeapon())
	{
		ActualWeaponOnTheHip->WeaponState.StartInteraction();
		PlayDataTableAnimation(GetOwnerCharacter(), EquipSwordAnimMontageDataTable, false, AnimAction::Equip);
	}
}

void UEquipmentComponent::SheathWeapon()
{
	if(WeaponInHands)
	{
	ULogger::Log(ELogLevel::WARNING, FString(("HAS STARTED INTERACTION: ") + FString(WeaponInHands->WeaponState.IsStartedInteraction ? "true" : "false")));
	}
	if (CanSheathWeapon())
	{
		WeaponInHands->WeaponState.StartInteraction();
		PlayDataTableAnimation(GetOwnerCharacter(), EquipSwordAnimMontageDataTable, false, AnimAction::Sheath);
	}
}

void UEquipmentComponent::AttachWeaponToHand()
{
	if (ActualWeaponOnTheHip)
	{
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
		WeaponInHands = ActualWeaponOnTheHip;
		ActualWeaponOnTheHip = nullptr;
	}
}

void UEquipmentComponent::AttachWeaponToThy()
{
	if (WeaponInHands)
	{
		WeaponInHands->AttachToComponent(
			GetOwnerCharacter()->GetMesh(),
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true
			),
			BoneSockets::HipWeaponSocket
		);
		ActualWeaponOnTheHip = WeaponInHands;
		WeaponInHands = nullptr;
	}
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HipSlotForWeapon)
	{
		ActualWeaponOnTheHip = GetWorld()->SpawnActor<AWeaponBase>(HipSlotForWeapon);
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
