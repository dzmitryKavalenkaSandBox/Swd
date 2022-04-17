#include "DamageConsumerComponent.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Utils/Logger.h"


UDamageConsumerComponent::UDamageConsumerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDamageConsumerComponent::ConsumeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                             AController* Instigator, AActor* DamageCauser)
{
	if (auto Owner = Cast<ASwdCharacter>(GetOwner()))
	{
		float NewHealthValue = FMath::Clamp(Owner->GetCurrentHealth() - Damage, 0.f, Owner->MaxHealth);
		Owner->UpdateCurrentHealth(NewHealthValue);
		if (Owner->GetCurrentHealth() <= 0.f)
		{
			auto DeadActor = Cast<IKillable>(DamagedActor);
			if (DeadActor)
			{
				DeadActor->HandleDeathBehavior();
			}
		}
	}
	else ULogger::Log(ELogLevel::ERROR, TEXT("Damage Inflictor Component does not have an owner"));
}


void UDamageConsumerComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UDamageConsumerComponent::ConsumeDamage);
}
