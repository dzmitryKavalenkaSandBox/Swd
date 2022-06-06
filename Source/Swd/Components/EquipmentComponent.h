#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Swd/Weapons/WeaponBase.h"
#include "EquipmentComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SWD_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	bool CanEquipWeapon();
	
	bool CanSheathWeapon();

	void EquipWeapon();
	
	void SheathWeapon();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	
	void AttachWeaponToThy();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	AWeaponBase* ActualWeaponOnTheHip = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	AWeaponBase* WeaponInHands = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UDataTable* EquipSwordAnimMontageDataTable = nullptr;

	void SetUpAnimations();

private:
	ASwdCharacter* GetCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	TSubclassOf<AWeaponBase> HipSlotForWeapon = nullptr;
};
