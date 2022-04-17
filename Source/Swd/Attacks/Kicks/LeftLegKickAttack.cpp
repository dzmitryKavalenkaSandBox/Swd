#include "LeftLegKickAttack.h"

ULeftLegKickAttack::ULeftLegKickAttack()
{
	AttackSource = EAttackSource::LEFT_LEG;
	AttackDamageFactor = .4f;
	AttackStaminaFactor = 15.f;
	// static ConstructorHelpers::FObjectFinder<USoundCue> SwingSoundCueObject(AttackSound::BasicSwingSound);
	// if (SwingSoundCueObject.Succeeded())
	// {
	// 	SwingSoundCue = SwingSoundCueObject.Object;
	// }
}

float ULeftLegKickAttack::GetAttackDamageFactor()
{
	return AttackDamageFactor;
}

float ULeftLegKickAttack::GetAttackStaminaFactor()
{
	return AttackStaminaFactor;
}

FString ULeftLegKickAttack::AttackName()
{
	return TEXT("LeftLegKickAttack");
}
