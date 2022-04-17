#include "FastSwingFromRight.h"

UFastSwingFromRight::UFastSwingFromRight()
{
	AttackDamageFactor = 1.f;
	AttackStaminaFactor = 10.f;
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
