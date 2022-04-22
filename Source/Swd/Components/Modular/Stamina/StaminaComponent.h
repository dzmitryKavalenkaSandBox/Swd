#pragma once

#include "CoreMinimal.h"
#include "Fatiguable.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentStamina;

	void DrainStamina(float AmountToDrain);

	void RestoreStamina();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	float CalculateDelayBeforeRestore() const;

	FTimerHandle StaminaRegenTimer;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float StaminaToRestorePerTick;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float DefaultDelayBeforeRestore;

	IFatiguable* Owner = nullptr; 
};
