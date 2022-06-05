#include "DamageInflictorComponent.h"

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


void UDamageInflictorComponent::InflictDamage(AActor* DamagedActor, float Damage, const FHitResult& HitResult,
                                              TSubclassOf<UDamageType> DamageType)
{
	if (auto Owner = Cast<ASwdCharacter>(GetOwner()))
	{
		UGameplayStatics::ApplyDamage(
			DamagedActor,
			Damage,
			Owner->GetController(),
			Owner,
			DamageType
		);
	}
	if (auto DamagedCharacter = Cast<ASwdCharacter>(DamagedActor))
	{
		DamagedCharacter->HitReaction(HitResult);
	}
	else ULogger::Log(ELogLevel::ERROR, TEXT("Damage Inflictor Component does not have an owner"));
}
