#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USphereComponent;
class UBoxComponent;
class USceneComponent;
class USoundCue;
class UAudioComponent;

USTRUCT(BlueprintType)
struct FWeaponState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	bool IsSwinging;

	void StartInteraction()
	{
		IsStartedInteraction = true;
		IsFinishedInteraction = false;
		IsBeingActedUpon = true;
	}

	void FinishInteraction()
	{
		IsStartedInteraction = false;
		IsFinishedInteraction = true;
		IsBeingActedUpon = false;
	}

	bool CanActOnWeapon() const
	{
		return !IsSwinging && !IsBeingActedUpon;
	}

	UPROPERTY(VisibleAnywhere)
	bool IsStartedInteraction = false;
private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess= true))
	bool IsBeingActedUpon = false;
	UPROPERTY(VisibleAnywhere)
	bool IsFinishedInteraction = true;
};

UCLASS()
class SWD_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	float GetWeaponBaseDamage();

	USceneComponent* GetWeaponStart();

	USceneComponent* GetWeaponEnd();

	/**
	 *	Area upon entering which Character can interact with weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* InteractionSphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	FWeaponState WeaponState;

	UFUNCTION()
	void PlayDrawWeaponSound();

	UFUNCTION()
	void PlaySheathWeaponSound();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sounds")
	USoundCue* DrawWeaponSoundCue = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sounds")
	USoundCue* SheathWeaponSoundCue = nullptr;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnInteractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                     const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float BaseDamage = 10.f;

	/**
	*	'Damaging' area of the weapon
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collision", meta=(AllowPrivateAccess=true))
	UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sounds", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* DrawWeaponSoundComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sounds", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* SheathWeaponSoundComponent = nullptr;
};
