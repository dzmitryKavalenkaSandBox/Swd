#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Swd/Interfacces/Killable.h"
#include "SwdCharacter.generated.h"

UCLASS(config=Game)
class ASwdCharacter : public ACharacter, public IKillable
{
	GENERATED_BODY()

public:
	ASwdCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEquipmentComponent* EquipmentComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageConsumerComponent* DamageConsumerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAttackComponent* AttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* LeftLegCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* RightLegCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageInflictorComponent* DamageInflictorComponent;

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

	UFUNCTION()
	virtual void HandleDeathBehavior() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnKickOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.f;

	float GetCurrentHealth() const;

	UFUNCTION()
	virtual void UpdateCurrentHealth(float NewValue);

	virtual void UpdateHealthOnWidget() const {}
	
	virtual void UpdateStaminaOnWidget() const {}

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float RunSpeed = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float RunSpeedCombat = 350.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float WalkSpeed = 200.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float WalkSpeedCombat = 180.f;

	UFUNCTION()
	void InitialMovementSetUp();

	UPROPERTY(BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess=true))
	float CurrentHealth;
};
