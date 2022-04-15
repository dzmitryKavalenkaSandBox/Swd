#include "AttackComponent.h"

#include "EquipmentComponent.h"
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
		if (AttackToPreform)
		{
			AttackToPreform->PerformAttack(Character);
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
	AttackToPreform = Attack.GetDefaultObject();
	AttackToPreform->Attacker = Cast<ASwdCharacter>(GetOwner());
}

void UAttackComponent::AttackStart()
{
	SwitchCollisionProfile(GetCurrentAttack()->AttackSource, "Weapon");
	if (auto CollisionBox = GetAttackSourceCollisionBox())
	{
		CollisionBox->SetNotifyRigidBodyCollision(true);
	}
}

void UAttackComponent::AttackEnd()
{
	SwitchCollisionProfile(GetCurrentAttack()->AttackSource, "NoCollision");
	if (auto CollisionBox = GetAttackSourceCollisionBox())
	{
		CollisionBox->SetNotifyRigidBodyCollision(false);
	}
}

void UAttackComponent::SwitchCollisionProfile(EAttackSource AttackSource, FName CollisionProfileName)
{
	if (auto Character = Cast<ASwdCharacter>(GetOwner()))
	{
		switch (AttackSource)
		{
		case EAttackSource::LEFT_LEG:
			{
				Character->LeftLegCollisionBox->SetCollisionProfileName(CollisionProfileName);
				break;
			}
		case EAttackSource::RIGHT_LEG:
			{
				Character->RightLegCollisionBox->SetCollisionProfileName(CollisionProfileName);
				break;
			}
		default: Character->LeftLegCollisionBox->SetCollisionProfileName(CollisionProfileName);
		}
	}
}

UBoxComponent* UAttackComponent::GetAttackSourceCollisionBox()
{
	auto Character = Cast<ASwdCharacter>(GetOwner());
	if (Character && AttackToPreform)
	{
		switch (AttackToPreform->AttackSource)
		{
		case EAttackSource::LEFT_LEG:
			{
				return Character->LeftLegCollisionBox;
			}
		case EAttackSource::RIGHT_LEG:
			{
				return Character->RightLegCollisionBox;
			}
		case EAttackSource::WEAPON:
			{
				if (Character->EquipmentComponent->WeaponInHands)
				{
					return Character->EquipmentComponent->WeaponInHands->CollisionBox;
				}
			}
		default: ULogger::Log(ELogLevel::ERROR, FString("Attack '") +
		                      AttackToPreform->AttackName() + FString("' has non AttackSource set"));
			break;
		}
	}
	return nullptr;
}

UAttackBase* UAttackComponent::GetCurrentAttack()
{
	return AttackToPreform/*.GetDefaultObject()*/;
}
