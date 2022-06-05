#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Swd/Interfacces/DataTableAnimated.h"
#include "DamageConsumerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UDamageConsumerComponent : public UActorComponent, public IDataTableAnimated
{
	GENERATED_BODY()

public:
	UDamageConsumerComponent();

	UFUNCTION()
	void ConsumeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                   AController* Instigator, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* HitAnimMontageDataTable = nullptr;

protected:
	virtual void BeginPlay() override;

public:
};
