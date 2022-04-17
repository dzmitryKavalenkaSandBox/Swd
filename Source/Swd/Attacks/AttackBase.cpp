#include "AttackBase.h"

#include "Sound/SoundCue.h"
#include "Swd/Swd.h"
#include "Swd/Components/AttackComponent.h"


UAttackBase::UAttackBase()
{
	// static ConstructorHelpers::FObjectFinder<UDataTable> AttackMontageDataObject(AM_DataTableReference::Attacks);
	// if (AttackMontageDataObject.Succeeded())
	// {
	// 	AttackAnimMontageDataTable = AttackMontageDataObject.Object;
	// }
}

void UAttackBase::PerformAttack(ASwdCharacter* Character)
{
	Character->AttackComponent->PlayDataTableAnimation(Character, AttackAnimMontageDataTable, HasBlendedAnim(),
	                                                   AttackName());
}

float UAttackBase::GetAttackDamageFactor()
{
	return 1.f;
}

float UAttackBase::GetAttackStaminaFactor()
{
	return 10.f;
}

bool UAttackBase::HasBlendedAnim()
{
	return false;
}

FString UAttackBase::AttackName()
{
	return this->StaticClass()->GetFName().ToString();
}
