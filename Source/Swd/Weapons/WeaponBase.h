#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Swd/Character/SwdCharacter.h"
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

	/**
	 *	Area upon entering which Character can interact with weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* InteractionSphere;

	/**
	 *	Start of the line trace
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* BladeDamagePointStart;

	/**
	 * End of a line trace
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* BladeDamagePointEnd;
	
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

	// /**
	// *	'Damaging' area of the weapon
	// */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collision", meta=(AllowPrivateAccess=true))
	// UBoxComponent* CollisionBox;

	UFUNCTION()
	void SetOwnerCharacter(ASwdCharacter* NewOwner);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnInteractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                     const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// UFUNCTION()
	// void OnCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	//                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	//                                 const FHitResult& SweepResult);

	ASwdCharacter* GetCharacter();
private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float BaseDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sounds", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* DrawWeaponSoundComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Sounds", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* SheathWeaponSoundComponent = nullptr;
};
