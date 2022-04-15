#include "RightLegKickAttack.h"

URightLegKickAttack::URightLegKickAttack()
{
	AttackSource = EAttackSource::RIGHT_LEG;
}

float URightLegKickAttack::AttackDamageFactor()
{
	return .4f;
}

FString URightLegKickAttack::AttackName()
{
	return TEXT("RightLegKickAttack");
}