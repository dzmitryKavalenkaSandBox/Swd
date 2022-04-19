#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class SWD_API ASword : public AWeaponBase
{
	GENERATED_BODY()

public:
	ASword();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collision", meta=(AllowPrivateAccess=true))
	UBoxComponent* PommelCollisionBox;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPommelCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float PommelBaseDamage = 5.f;
};
