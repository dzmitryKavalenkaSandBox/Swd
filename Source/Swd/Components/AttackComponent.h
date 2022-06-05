#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Swd/Interfacces/DataTableAnimated.h"
#include "AttackComponent.generated.h"

class UAttackData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UAttackComponent : public UActorComponent, public IDataTableAnimated
{
	GENERATED_BODY()

public:
	UAttackComponent();

	UFUNCTION(BlueprintCallable)
	void PerformAttackAnimation();

	UFUNCTION(BlueprintCallable)
	void SetAttackToPerform(UAttackData* Attack);

	void AttackStart();

	void AttackEnd();

	UFUNCTION()
	UAttackData* GetCurrentAttack();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UAttackData* AttackToPreform = nullptr;

	ASwdCharacter* GetCharacter();

	FHitResult CastAttackTrace(FVector TraceStart, FVector TraceEnd, FCollisionQueryParams CollisionParams);

	void DrawDebugTraceLine(FVector TraceStart, FVector TraceEnd);

	void HandleTraceHit(FHitResult HitResult, float BaseDamage);

	void AttackTraceOnTickAndHandleHitResult();
};
