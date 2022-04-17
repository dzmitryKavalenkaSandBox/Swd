#include "DamageInflictorComponent.h"

#include "AttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Swd/Character/SwdCharacter.h"
#include "Swd/Utils/Logger.h"


UDamageInflictorComponent::UDamageInflictorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UDamageInflictorComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDamageInflictorComponent::InflictDamage(AActor* DamagedActor, float Damage, TSubclassOf<UDamageType> DamageType)
{
	if (auto Owner = Cast<ASwdCharacter>(GetOwner()))
	{
		if (auto Attack = Owner->AttackComponent->GetCurrentAttack())
		{
			UGameplayStatics::ApplyDamage(
				DamagedActor,
				BaseDamage * Damage,
				Owner->GetController(),
				Owner,
				DamageType
			);
		}
		else ULogger::Log(ELogLevel::ERROR, TEXT("Trying to apply damage with no attack assigned on Attack Component"));
	}
	else ULogger::Log(ELogLevel::ERROR, TEXT("Damage Inflictor Component does not have an owner"));
}
