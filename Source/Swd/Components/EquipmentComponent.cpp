#include "EquipmentComponent.h"

#include "Swd/Swd.h"
#include "Swd/Character/SwdCharacter.h"
#include "Engine/DataTable.h"
#include "Swd/Animations/Animinstances/AnimInstanceBase.h"
#include "Swd/DataAssets/CharacterData.h"
#include "Swd/Utils/Logger.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetUpAnimations();
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HipSlotForWeapon)
	{
		ActualWeaponOnTheHip = GetWorld()->SpawnActor<AWeaponBase>(HipSlotForWeapon);
		ActualWeaponOnTheHip->SetOwner(GetCharacter());
		ActualWeaponOnTheHip->WeaponSkeletalMesh->SetSimulatePhysics(false);
		ActualWeaponOnTheHip->AttachToComponent(
			GetCharacter()->GetMesh(),
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
		PlayDataTableAnimation(GetCharacter(), EquipSwordAnimMontageDataTable, true, AnimAction::Equip);
	}
}

void UEquipmentComponent::SheathWeapon()
{
	if (CanSheathWeapon())
	{
		WeaponInHands->WeaponState.StartInteraction();
		PlayDataTableAnimation(GetCharacter(), EquipSwordAnimMontageDataTable, true, AnimAction::Sheath);
	}
}

void UEquipmentComponent::AttachWeaponToHand()
{
	if (ActualWeaponOnTheHip)
	{
		ActualWeaponOnTheHip->AttachToComponent(
			GetCharacter()->GetMesh(),
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true
			),
			BoneSockets::WeaponHandle
		);
		WeaponInHands = ActualWeaponOnTheHip;
		WeaponInHands->PlayDrawWeaponSound();
		ActualWeaponOnTheHip = nullptr;
		if (auto AnimInstance = GetCharacter()->GetAnimInstance())
		{
			ULogger::Log(ELogLevel::INFO, TEXT("AnimInstance->bHasWeaponEquipped = true;"));
			AnimInstance->bHasWeaponEquipped = true;
		}
	}
}

void UEquipmentComponent::AttachWeaponToThy()
{
	if (WeaponInHands)
	{
		WeaponInHands->AttachToComponent(
			GetCharacter()->GetMesh(),
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				true
			),
			BoneSockets::HipWeaponSocket
		);
		ActualWeaponOnTheHip = WeaponInHands;
		ActualWeaponOnTheHip->PlaySheathWeaponSound();
		WeaponInHands = nullptr;
		if (auto AnimInstance = GetCharacter()->GetAnimInstance())
		{
			ULogger::Log(ELogLevel::INFO, TEXT("AnimInstance->bHasWeaponEquipped = false;"));
			AnimInstance->bHasWeaponEquipped = false;
		}
	}
}

void UEquipmentComponent::SetUpAnimations()
{
	/*static ConstructorHelpers::FObjectFinder<UDataTable> EquipWeaponDataObject(
		AM_DataTableReference::EquipSheathWeapon);
	if (EquipWeaponDataObject.Succeeded())
	{
		EquipSwordAnimMontageDataTable = EquipWeaponDataObject.Object;
	}*/
}

ASwdCharacter* UEquipmentComponent::GetCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}
