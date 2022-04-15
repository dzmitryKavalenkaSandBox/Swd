#include "RightLegKickAttack.h"

URightLegKickAttack::URightLegKickAttack()
{
}

float URightLegKickAttack::AttackDamageFactor()
{
	return .4f;
}

FString URightLegKickAttack::AttackName()
{
	return TEXT("RightLegKickAttack");
}