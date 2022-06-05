#pragma once

#include "CoreMinimal.h"
#include "AttackData.generated.h"

enum class EAttackSource : uint8;
class UAnimMontage;
class ASwdCharacter;

UCLASS()
class SWD_API UAttackData : public UDataAsset
{
	GENERATED_BODY()
public:

	void PerformAttackAnimation(ASwdCharacter* Character);

	/*
	 * The Character that performs the attack
	 */
	UPROPERTY()
	ASwdCharacter* Attacker = nullptr;

	/*
	 * The Weapon or part of the body attack is performed by
	 */
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	EAttackSource AttackSource;

	/**
	 * How much damage this attack deals by default
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float AttackDamageFactor = 1.f;

	/**
	 * How much stamina this attack drains by default
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float AttackStaminaFactor = 10.f;

	/**
	 * Will be used to find the correct Anim Section on Attack Animation montage to play.
	 * Make sure the value of this field is equal to the Montage sections inside provided Attack Montages
	 */
	UPROPERTY(EditAnywhere)
	FString AnimSectionName;

protected:

	/**
	 * Attack animation to play (FullBody slot)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AttackMontage;

	/**
	* Attack animation to play (Upper slot)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* BlendedAttackMontage;
};
