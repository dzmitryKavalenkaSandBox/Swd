#pragma once

#include "CoreMinimal.h"
#include "Swd/Attacks/AttackBase.h"
#include "UObject/Object.h"
#include "PommelAttack.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SWD_API UPommelAttack : public UAttackBase
{
	GENERATED_BODY()

	UPommelAttack();

	virtual FString AttackName() override;

	virtual float GetAttackDamageFactor() override;

	virtual float GetAttackStaminaFactor() override;
};