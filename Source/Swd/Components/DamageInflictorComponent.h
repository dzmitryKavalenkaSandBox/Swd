#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageInflictorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UDamageInflictorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamageInflictorComponent();

	UFUNCTION()
	void InflictDamage(AActor* DamagedActor, float Damage, TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass());

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float BaseDamage = 10.f;
};