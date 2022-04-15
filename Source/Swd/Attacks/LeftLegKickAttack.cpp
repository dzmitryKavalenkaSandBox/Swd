
#include "LeftLegKickAttack.h"

ULeftLegKickAttack::ULeftLegKickAttack()
{
	// static ConstructorHelpers::FObjectFinder<USoundCue> SwingSoundCueObject(AttackSound::BasicSwingSound);
	// if (SwingSoundCueObject.Succeeded())
	// {
	// 	SwingSoundCue = SwingSoundCueObject.Object;
	// }
}

float ULeftLegKickAttack::AttackDamageFactor()
{
	return .4f;
}

FString ULeftLegKickAttack::AttackName()
{
	return TEXT("LeftLegKickAttack");
}