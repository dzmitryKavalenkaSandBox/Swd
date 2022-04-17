#include "RightLegKickAttack.h"

URightLegKickAttack::URightLegKickAttack()
{
	AttackSource = EAttackSource::RIGHT_LEG;
}

float URightLegKickAttack::GetAttackDamageFactor()
{
	return AttackDamageFactor;
}

float URightLegKickAttack::GetAttackStaminaFactor()
{
	return AttackStaminaFactor;
}

FString URightLegKickAttack::AttackName()
{
	return TEXT("RightLegKickAttack");
}
