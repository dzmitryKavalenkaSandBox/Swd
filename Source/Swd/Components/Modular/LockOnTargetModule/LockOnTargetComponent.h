#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnTargetComponent.generated.h"

class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API ULockOnTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockOnTargetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void RotateCharacterToTarget(FVector& LocationOfTarget);

	UFUNCTION(BlueprintCallable)
	void RotateCameraToTarget(FVector LocationOfTarget);

	void SetUpComponent(USpringArmComponent* SpringArm, bool ShouldRotateOnTick = true);

	UFUNCTION(BlueprintCallable)
	void SetTargetToLockOn(AActor* Target);

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentTarget();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	AActor* CurrentTarget = nullptr;

	UPROPERTY()
	ACharacter* Owner = nullptr;

	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	bool bRotateOnTick;
};
