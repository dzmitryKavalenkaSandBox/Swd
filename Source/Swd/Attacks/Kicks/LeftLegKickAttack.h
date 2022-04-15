#pragma once

#include "CoreMinimal.h"
#include "Swd/Attacks/AttackBase.h"
#include "LeftLegKickAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SWD_API ULeftLegKickAttack: public UAttackBase
{
	GENERATED_BODY()
	
	ULeftLegKickAttack();
	virtual FString AttackName() override;
	virtual float AttackDamageFactor() override;
	virtual float AttackStaminaFactor() override;
};
