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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	FWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collision", meta=(AllowPrivateAccess=true))
	UBoxComponent* CollisionBox;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	USceneComponent* WeaponStart;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	USceneComponent* WeaponEnd;
};
