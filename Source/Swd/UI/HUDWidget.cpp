#include "HUDWidget.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/AttackComponent.h"

// void UHUDWidget::OnLeftLegClicked()
// {
// 	auto Character = Cast<ASwdCharacter>(GetOwningPlayer());
//
// 	if(Character)
// 	{
// 		Character->AttackComponent->SetAttackToPerform()
// 	}
// }

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// LeftLeg->OnClicked.AddDynamic(this, &UHUDWidget::OnLeftLegClicked);
}

void UHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
