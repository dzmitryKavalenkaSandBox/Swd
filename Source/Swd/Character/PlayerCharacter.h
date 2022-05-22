#pragma once

#include "CoreMinimal.h"
#include "SwdCharacter.h"
#include "GameFramework/Actor.h"
#include "Swd/Components/Modular/NearestTargetFinderModule/ClosestActorFinderComponent.h"
#include "PlayerCharacter.generated.h"

class ULockOnTargetComponent;
UCLASS()
class SWD_API APlayerCharacter : public ASwdCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void UpdateHealthOnWidget() const override;

	virtual void UpdateStaminaOnWidget() const override;

	virtual void UpdateCurrentHealth(float NewValue) override;

	virtual void ManageCombatState(bool bEnableCombat) override;

	// virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// /** Handler for when a touch input begins. */
	// void OnTouchBegin(ETouchIndex::Type TouchType, FVector NewTouchLocation);

	// /** Handler for when a touch input stops. */
	// void OnTouchEnd(ETouchIndex::Type TouchType, FVector NewTouchLocation);

	virtual void InitialMovementSetUp() override;
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UWidgetComponent* HUDWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UWidgetComponent* HealthStaminaWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ULockOnTargetComponent* LockOnTargetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UClosestActorFinderComponent* ClosestActorFinderComponent;

	void SetUpHUDWidget();

	void SetUpHealthStaminaWidget();

	void SwitchTargetToLockOn();
	
	UPROPERTY()
	FTimerHandle RelaxedTimer;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float MinTimeBeforeCanRelax = 5;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float MaxTimeBeforeCanRelax = 10;
};
