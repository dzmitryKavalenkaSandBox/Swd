#include "PommelAttack.h"

UPommelAttack::UPommelAttack()
{
	AttackDamageFactor = .4f;
	AttackStaminaFactor = 5.f;
	AttackSource = EAttackSource::POMMEL;
}

float UPommelAttack::GetAttackDamageFactor()
{
	return AttackDamageFactor;
}

float UPommelAttack::GetAttackStaminaFactor()
{
	return AttackStaminaFactor;
}

FString UPommelAttack::AttackName()
{
	return TEXT("PommelAttack");
}

