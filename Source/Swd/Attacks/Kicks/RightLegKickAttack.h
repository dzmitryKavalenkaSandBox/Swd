#pragma once

#include "CoreMinimal.h"
#include "Swd/Attacks/AttackBase.h"
#include "RightLegKickAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SWD_API URightLegKickAttack : public UAttackBase
{
	GENERATED_BODY()

	URightLegKickAttack();

	virtual FString AttackName() override;

	virtual float GetAttackDamageFactor() override;

	virtual float GetAttackStaminaFactor() override;
};
