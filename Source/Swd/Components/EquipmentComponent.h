#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Swd/Interfacces/AMDataTablePlayable.h"
#include "Swd/Weapons/WeaponBase.h"
#include "EquipmentComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UEquipmentComponent : public UActorComponent, public IAMDataTablePlayable
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	bool CanEquipWeapon();

	void EquipWeapon();

	void AttachWeaponToHand();

protected:
	virtual void BeginPlay() override;

public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	AWeaponBase* ActualWeaponOnTheHip = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UDataTable* EquipSwordAnimMontageDataTable = nullptr;

	void SetUpAnimations();

private:
	ASwdCharacter* GetOwnerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	TSubclassOf<AWeaponBase> WeaponOnTheHipSlot = nullptr;
};
