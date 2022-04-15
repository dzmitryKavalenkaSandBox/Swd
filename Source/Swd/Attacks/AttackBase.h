#pragma once

#include "CoreMinimal.h"
#include "Swd/Interfacces/AMDataTablePlayable.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

class USoundCue;
class UAudioComponent;
class UDataTable;
/**
 * 
 */
UCLASS()
class SWD_API UAttackBase : public UObject, public IAMDataTablePlayable
{
	GENERATED_BODY()

public:
	UAttackBase();
	virtual void PerformAttack(ASwdCharacter* Character);
	virtual float AttackDamageFactor();	
	virtual float AttackStaminaFactor();
	virtual bool HasBlendedAnim();
	virtual FString AttackName();

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// USoundCue* SwingSoundCue = nullptr;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* AttackAnimMontageDataTable = nullptr;
};
