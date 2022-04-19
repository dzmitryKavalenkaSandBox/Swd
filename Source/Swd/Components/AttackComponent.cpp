#include "AttackComponent.h"

#include "EquipmentComponent.h"
#include "StaminaComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Swd/Swd.h"
#include "Swd/Utils/Logger.h"
#include "Swd/Weapons/Sword.h"


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
	if (Attack)
	{
		AttackToPreform = Attack.GetDefaultObject();
		AttackToPreform->Attacker = Cast<ASwdCharacter>(GetOwner());
	}
}

void UAttackComponent::AttackStart()
{
	SwitchCollisionProfile(CollisionProfile::Weapon);

	if (auto CollisionBox = GetAttackSourceCollisionBox())
	{
		CollisionBox->SetNotifyRigidBodyCollision(true);
		CollisionBox->SetGenerateOverlapEvents(true);
	}
}

void UAttackComponent::AttackEnd()
{
	SwitchCollisionProfile(CollisionProfile::NoCollision);
	if (auto CollisionBox = GetAttackSourceCollisionBox())
	{
		CollisionBox->SetNotifyRigidBodyCollision(false);
		CollisionBox->SetGenerateOverlapEvents(false);
	}
	GetCharacter()->StaminaComponent->DrainStamina(GetCurrentAttack()->GetAttackStaminaFactor());
}

void UAttackComponent::SwitchCollisionProfile(FName CollisionProfileName)
{
	if (auto CollisionBox = GetAttackSourceCollisionBox())
	{
		CollisionBox->SetCollisionProfileName(CollisionProfileName);
	}
}

UBoxComponent* UAttackComponent::GetAttackSourceCollisionBox()
{
	auto Character = GetCharacter();
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
				ULogger::Log(ELogLevel::ERROR,
				             TEXT("Trying to perfrom attach with weapon having no Weapon in Hands"));
				return nullptr;
			}
		case EAttackSource::POMMEL:
			{
				if (auto Sword = Cast<ASword>(Character->EquipmentComponent->WeaponInHands))
				{
					return Sword->PommelCollisionBox;
				}
				ULogger::Log(ELogLevel::ERROR,
				             TEXT("Trying to perfrom attach with sword having no Sword in Hands"));
				return nullptr;
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
	return AttackToPreform;
}

ASwdCharacter* UAttackComponent::GetCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}
