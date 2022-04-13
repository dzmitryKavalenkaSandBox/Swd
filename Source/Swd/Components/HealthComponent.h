#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	void UpdateHealthOnWidgets(AActor* DamagedActor);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	
protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoundCue* HitSoundCue;
	UPROPERTY()
	class UAudioComponent* HitSoundComponent;
	
	UFUNCTION()
	void DamageTaken(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		// controller responsible for damage
		class AController* Instigator,
		AActor* DamageCauser
	);
};
