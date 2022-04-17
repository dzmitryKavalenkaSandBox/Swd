#pragma once

#include "CoreMinimal.h"
#include "Swd/Interfacces/AMDataTablePlayable.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

UENUM(BlueprintType)
enum class EAttackSource: uint8
{
	NONE UMETA(DisplayName = "NONE"),
	RIGHT_LEG UMETA(DisplayName = "Right Leg"),
	LEFT_LEG UMETA(DisplayName = "Left Leg"),
	WEAPON UMETA(DisplayName = "Weapon"),
};

class USoundCue;
class UAudioComponent;
class UDataTable;
class ASwdCharacter;

UCLASS()
class SWD_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UAttackBase();
	virtual void PerformAttack(ASwdCharacter* Character);
	virtual float GetAttackDamageFactor();
	virtual float GetAttackStaminaFactor();
	virtual bool HasBlendedAnim();
	virtual FString AttackName();

	/*
	 * The Weapon or part of the body attack is performed by
	 */
	EAttackSource AttackSource;
	/*
	 * The Character that performs the attack
	 */
	UPROPERTY()
	ASwdCharacter* Attacker = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* AttackAnimMontageDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float AttackDamageFactor;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float AttackStaminaFactor;
};
