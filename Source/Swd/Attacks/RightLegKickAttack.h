// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "UObject/Object.h"
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
	virtual float AttackDamageFactor() override;
};
