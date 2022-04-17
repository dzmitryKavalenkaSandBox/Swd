#pragma once

#include "CoreMinimal.h"
#include "Swd/Attacks/AttackBase.h"
#include "FastSwingFromRight.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SWD_API UFastSwingFromRight : public UAttackBase
{
	GENERATED_BODY()

	UFastSwingFromRight();

	virtual FString AttackName() override;

	virtual float GetAttackDamageFactor() override;

	virtual float GetAttackStaminaFactor() override;
	
	virtual bool HasBlendedAnim() override;
};
