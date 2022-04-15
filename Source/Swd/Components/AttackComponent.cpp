#include "AttackComponent.h"

#include "Components/BoxComponent.h"
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

void UAttackComponent::SwitchCollisionProfile(EAttackSource AttackSource, FName CollisionProfileName)
{
	if (auto Character = Cast<ASwdCharacter>(GetOwner()))
	{
		switch (AttackSource)
		{
		case EAttackSource::LEFT_LEG:
			{
				ULogger::Log(ELogLevel::WARNING, "Setting collision to Left leg to " + CollisionProfileName.ToString());
				Character->LeftLegCollisionBox->SetCollisionProfileName(CollisionProfileName);
				break;
			}
		case EAttackSource::RIGHT_LEG:
			{
				ULogger::Log(ELogLevel::WARNING, "Setting collision to Right leg to " + CollisionProfileName.ToString());
				Character->RightLegCollisionBox->SetCollisionProfileName(CollisionProfileName);
				break;
			}
		default: Character->LeftLegCollisionBox->SetCollisionProfileName(CollisionProfileName);
		}
	}
}

UAttackBase* UAttackComponent::GetCurrentAttack()
{
	return AttackToPreform.GetDefaultObject();
}
