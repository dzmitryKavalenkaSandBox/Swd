// Fill out your copyright notice in the Description page of Project Settings.


#include "SheathWeaponNotifyState.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/EquipmentComponent.h"

void USheathWeaponNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	if (Character->EquipmentComponent->WeaponInHands)
	{
		Character->EquipmentComponent->AttachWeaponToThy();
	}
}

void USheathWeaponNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	if (auto Weapon = Character->EquipmentComponent->ActualWeaponOnTheHip)
	{
		Weapon->WeaponState.FinishInteraction();
	}
}
