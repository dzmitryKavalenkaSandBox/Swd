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

protected:
	virtual void BeginPlay() override;
};
