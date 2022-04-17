#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UPROPERTY(VisibleAnywhere)
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentStamina;

	void DrainStamina(float AmountToDrain);

	void RestoreStamina();
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	float CalculateDelayBeforeRestore() const;
	void UpdateStaminaOnWidgets();

	UPROPERTY(VisibleAnywhere)
	FTimerHandle StaminaRegenTimer;
	UPROPERTY(VisibleAnywhere)
	float StaminaToRestorePerTick;
	UPROPERTY(VisibleAnywhere)
	float DefaultDelayBeforeRestore;
};