#pragma once

#include "CoreMinimal.h"
#include "CharacterData.h"
#include "Swd/Swd.h"
#include "AICharacterData.generated.h"

UCLASS()
class SWD_API UAICharacterData : public UCharacterData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere)
	ECombatRole CombatRole;

	/*
	 * Animinstace class to use when AI is alerted
	 */
	UPROPERTY(EditAnywhere, Category = "Animation")
	UClass* AlertedAnimInstanceClass = nullptr;
};
