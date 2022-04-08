#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USphereComponent;
class UBoxComponent;
class USceneComponent;

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

private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess= true))
	bool IsBeingActedUpon = false;
	UPROPERTY(VisibleAnywhere)
	bool IsStartedInteraction = false;
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
};
