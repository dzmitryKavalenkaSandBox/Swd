#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Swd/Components/Modular/Stamina/Fatiguable.h"
#include "Swd/Interfacces/Killable.h"
#include "SwdCharacter.generated.h"

enum class EFaction : uint8;
UCLASS(config=Game)
class ASwdCharacter : public ACharacter, public IKillable, public IFatiguable
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
	class USphereComponent* LeftLegEndTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* RightLegCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* RightLegEndTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageInflictorComponent* DamageInflictorComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Animation")
	bool bIsAnimationBlended;

	UFUNCTION(BlueprintPure, Category="Movement")
	float GetSpeed();

	UFUNCTION(BlueprintCallable)
	void EquipSheathWeapon();

	UFUNCTION()
	virtual void ManageCombatState(bool bEnableCombat);

	UFUNCTION(BlueprintPure)
	bool GetIsInCombat() const;

	UFUNCTION()
	virtual void HandleDeathBehavior() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// UFUNCTION()
	// void OnKickOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	//                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	//                         const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.f;

	float GetCurrentHealth() const;

	UFUNCTION()
	virtual void UpdateCurrentHealth(float NewValue);

	virtual void UpdateHealthOnWidget() const
	{
	}

	virtual void UpdateStaminaOnWidget() const override
	{
	}

	bool IsHostile(ASwdCharacter* Character);

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	EFaction Faction;

	UFUNCTION(BlueprintCallable)
	void MakeANoise(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	bool bIsInputEnabled = true;

	void ManageInput(const bool bShouldEnable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UCharacterData* CharacterData;

	class UAnimInstanceBase* GetAnimInstance();

	virtual void AtEase();
	
	virtual void Ready();

	UFUNCTION()
	virtual void InitialMovementSetUp();

	UFUNCTION(BlueprintImplementableEvent)
	void SetupPhysicalAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void HitReaction(FHitResult HitResult);

	UPROPERTY(EditAnywhere, Category="Attack")
	float KickBaseDamage = 10.f;

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

	UPROPERTY(BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess=true))
	float CurrentHealth;
};
