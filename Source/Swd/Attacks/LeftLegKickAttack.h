#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "UObject/Object.h"
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
};
