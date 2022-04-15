
#include "FastSwingFromRight.h"

UFastSwingFromRight::UFastSwingFromRight()
{
	AttackSource = EAttackSource::WEAPON;
}

FString UFastSwingFromRight::AttackName()
{
	return TEXT("FastSwingFromRight");
}

float UFastSwingFromRight::AttackDamageFactor()
{
	return .5;
}

float UFastSwingFromRight::AttackStaminaFactor()
{
	return 10.f;
}

bool UFastSwingFromRight::HasBlendedAnim()
{
	return true;
}
