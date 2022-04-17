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

	virtual float GetAttackDamageFactor() override;
	
	virtual float GetAttackStaminaFactor() override;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float AttackDamageFactor = .4f;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float AttackStaminaFactor = 15.f;
};
