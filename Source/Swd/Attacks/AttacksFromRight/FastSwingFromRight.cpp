
#include "FastSwingFromRight.h"

UFastSwingFromRight::UFastSwingFromRight()
{
	AttackSource = EAttackSource::WEAPON;
}

FString UFastSwingFromRight::AttackName()
{
	return TEXT("FastSwingFromRight");
}

float UFastSwingFromRight::GetAttackDamageFactor()
{
	return AttackDamageFactor;
}

float UFastSwingFromRight::GetAttackStaminaFactor()
{
	return AttackStaminaFactor;
}

bool UFastSwingFromRight::HasBlendedAnim()
{
	return true;
}
