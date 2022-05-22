#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

UCLASS()
class SWD_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	bool bIsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	bool bIsAnimationBlended;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	bool bIsInCombat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	bool bIsWeaponSheathed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	bool bIsIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim Instace")
	bool bHasWeaponEquipped;

	UAnimInstanceBase();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class ASwdCharacter* Character;
};
