#include "RightLegKickAttack.h"

URightLegKickAttack::URightLegKickAttack()
{
	AttackSource = EAttackSource::RIGHT_LEG;
}

float URightLegKickAttack::AttackDamageFactor()
{
	return .4f;
}

float URightLegKickAttack::AttackStaminaFactor()
{
	return 15.f;
}

FString URightLegKickAttack::AttackName()
{
	return TEXT("RightLegKickAttack");
}
