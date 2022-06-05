#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Swd/Attacks/AttackBase.h"
#include "Swd/Interfacces/DataTableAnimated.h"
#include "AttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UAttackComponent : public UActorComponent, public IDataTableAnimated
{
	GENERATED_BODY()

public:
	UAttackComponent();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void SetAttackToPerform(TSubclassOf<UAttackBase> Attack);
	
	void AttackStart();

	void AttackEnd();

	UFUNCTION()
	UAttackBase* GetCurrentAttack();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UAttackBase* AttackToPreform = nullptr;

	UFUNCTION()
	void SwitchCollisionProfile(FName CollisionProfileName);
	
	UBoxComponent* GetAttackSourceCollisionBox();
	
	ASwdCharacter* GetCharacter();
	
	FHitResult SwingForAttack(FVector TraceStart, FVector TraceEnd, FCollisionQueryParams CollisionParams);
	
	void DrawDebugTraceLine(FVector TraceStart, FVector TraceEnd);
	
	void HandleTraceHit(FHitResult HitResult, float BaseDamage);
	
	void DoAttackTrace();
};
