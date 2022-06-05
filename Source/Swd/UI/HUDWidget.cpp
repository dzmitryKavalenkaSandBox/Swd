#include "HUDWidget.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/AttackComponent.h"

void UHUDWidget::OnRightElbowPressed()
{
	if (OwningPlayer)
	{
		OwningPlayer->AttackComponent->SetAttackToPerform(PommelAttack);
		OwningPlayer->AttackComponent->PerformAttackAnimation();
	}
}

void UHUDWidget::OnLeftLegPressed()
{
	if (OwningPlayer)
	{
		OwningPlayer->AttackComponent->SetAttackToPerform(LeftLegKickAttack);
		OwningPlayer->AttackComponent->PerformAttackAnimation();
	}
}

void UHUDWidget::OnRightLegPressed()
{
	if (OwningPlayer)
	{
		OwningPlayer->AttackComponent->SetAttackToPerform(RightLegKickAttack);
		OwningPlayer->AttackComponent->PerformAttackAnimation();
	}
}

void UHUDWidget::OnRightHandPressed()
{
	if (OwningPlayer)
	{
		OwningPlayer->AttackComponent->SetAttackToPerform(RightHandAttack);
		OwningPlayer->AttackComponent->PerformAttackAnimation();
	}
}

void UHUDWidget::OnLeftHandPressed()
{
	if (OwningPlayer)
	{
		OwningPlayer->EquipSheathWeapon();
	}
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OwningPlayer = Cast<ASwdCharacter>(GetOwningPlayer()->GetPawn());
}

void UHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LeftLeg->OnPressed.AddDynamic(this, &UHUDWidget::OnLeftLegPressed);
	RightLeg->OnPressed.AddDynamic(this, &UHUDWidget::OnRightLegPressed);
	RightHand->OnPressed.AddDynamic(this, &UHUDWidget::OnRightHandPressed);
	LeftHand->OnPressed.AddDynamic(this, &UHUDWidget::OnLeftHandPressed);
	RightElbow->OnPressed.AddDynamic(this, &UHUDWidget::OnRightElbowPressed);
}
