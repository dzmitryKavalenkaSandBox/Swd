#include "AttackComponent.h"

#include "Swd/Utils/Logger.h"


UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::Attack()
{
	if (auto Character = Cast<ASwdCharacter>(GetOwner()))
	{
		auto Attack = Cast<UAttackBase>(AttackToPreform->GetDefaultObject());
		if (Attack)
		{
			ULogger::Log(ELogLevel::WARNING, FString("Attack name: ") + Attack->GetName());
			Attack->PerformAttack(Character);
		}
	}
}


void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttackComponent::SetAttackToPerform(TSubclassOf<UAttackBase> Attack)
{
	AttackToPreform = Attack;
}
