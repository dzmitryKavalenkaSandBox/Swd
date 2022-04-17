#include "StaminaComponent.h"

#include "Swd/Character/AICharacterBase.h"
#include "Swd/Utils/Logger.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxStamina = 100.f;
	CurrentStamina = MaxStamina;
	StaminaToRestorePerTick = 20;
	DefaultDelayBeforeRestore = 2;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStaminaComponent::DrainStamina(float AmountToDrain)
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	CurrentStamina = FMath::Clamp(CurrentStamina - AmountToDrain, 0.f, MaxStamina);
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &UStaminaComponent::RestoreStamina,
	                                       1.f, true, CalculateDelayBeforeRestore());
	GetCharacter()->UpdateStaminaOnWidget();
}

void UStaminaComponent::RestoreStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaToRestorePerTick, 0.f, MaxStamina);
	if (CurrentStamina == MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	}
	GetCharacter()->UpdateStaminaOnWidget();
}

float UStaminaComponent::CalculateDelayBeforeRestore() const
{
	if (CurrentStamina == 0)
	{
		return DefaultDelayBeforeRestore * 2.f;
	}
	return DefaultDelayBeforeRestore;
}
//
// void UStaminaComponent::UpdateStaminaOnWidgets()
// {
// 	if (auto AICharacter = Cast<AAICharacterBase>(GetOwner()))
// 	{
// 		AICharacter->UpdateStaminaOnWidget();
// 	}
// 	// if (auto PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
// 	// {
// 	// 	PlayerCharacter->UpdateHUDWidget();
// 	// }
// }

ASwdCharacter* UStaminaComponent::GetCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}
