// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwdCharacter.h"
#include "Swd/Swd.h"
#include "AICharacterBase.generated.h"

USTRUCT(BlueprintType)
struct FAnimValues
{
	GENERATED_BODY()

public:
	// UPROPERTY(BlueprintReadWrite, Category = Movement)
	// bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsArmed = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsShooting = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bADS = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsSitting = false;
};

/**
 * 
 */
class UWidgetComponent;
class USphereComponent;
UCLASS()
class SWD_API AAICharacterBase : public ASwdCharacter
{
	GENERATED_BODY()

	AAICharacterBase();

	virtual void BeginPlay() override;
public:
	virtual void UpdateCurrentHealth(float NewValue) override;

	virtual void UpdateHealthOnWidget() const override;

	virtual void UpdateStaminaOnWidget() const override;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* TreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class ASmartObject* SmartObject;

	UPROPERTY(BlueprintReadOnly)
	class AAIControllerBase* ControllerRef = nullptr;

	// Custom View Target For AI Perception Component
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation,
	                           int32& NumberOfLoSChecksPerformed,
	                           float& OutSightStrength, const AActor* IgnoreActor = nullptr,
	                           const bool* bWasVisible = nullptr,
	                           int32* UserData = nullptr) const;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool Dead = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	FName PerceptionTarget = "spine_02";

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	ECombatRole CombatRole;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidgetRef();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidgetVis(bool Newbool);

	UFUNCTION(BlueprintCallable)
	void ToggleCombat(const bool ShouldEnableCombat);

	UFUNCTION(BlueprintCallable)
	void ToggleArmedState(const bool ShouldArmSelf);

	// UFUNCTION(BlueprintCallable)
	// void ToggleCrouch(const bool Newbool);

	UFUNCTION(BlueprintCallable)
	void ToggleADS(const bool Newbool);

	UFUNCTION(BlueprintCallable)
	void ToggleSprinting(bool bShouldSprint);

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FAnimValues AnimValues;

protected:
	virtual void InitialMovementSetUp() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UWidgetComponent* HealthStaminaWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UWidgetComponent* DetectorWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	USphereComponent* SphereAroundAI;

	void SetUpHealthStaminaWidget();

	void SetUpDetectorWidget();

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
