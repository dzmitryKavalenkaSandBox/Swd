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
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAttackComponent* AttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* LeftLegCollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* RightLegCollisionBox;

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

	void OnAttackHit(UPrimitiveComponent OnComponentHit, UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	virtual void BeginPlay() override;

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
};
