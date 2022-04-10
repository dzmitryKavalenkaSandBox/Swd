#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SwdCharacter.generated.h"

UCLASS(config=Game)
class ASwdCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASwdCharacter();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UEquipmentComponent* EquipmentComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Animation")
	bool bIsAnimationBlended;

	float GetSpeed();

	UFUNCTION(BlueprintCallable)
	void EquipSheathWeapon();

	UFUNCTION()
	void ManageCombatState(bool bEnableCombat);

	UFUNCTION(BlueprintPure)
	bool GetIsInCombat() const;

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess=true))
	bool bIsInCombat = false;
};
