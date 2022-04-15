#pragma once

#include "CoreMinimal.h"
#include "Swd/Interfacces/AMDataTablePlayable.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

UENUM(BlueprintType)
enum class EAttackSource: uint8
{
	RIGHT_LEG UMETA(DisplayName = "Right Leg"),
	LEFT_LEG UMETA(DisplayName = "Left Leg")
};

class USoundCue;
class UAudioComponent;
class UDataTable;
/**
 * 
 */
UCLASS()
class SWD_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:
	UAttackBase();
	virtual void PerformAttack(ASwdCharacter* Character);
	virtual float AttackDamageFactor();
	virtual float AttackStaminaFactor();
	virtual bool HasBlendedAnim();
	virtual FString AttackName();

	EAttackSource AttackSource;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// USoundCue* SwingSoundCue = nullptr;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* AttackAnimMontageDataTable = nullptr;
};
