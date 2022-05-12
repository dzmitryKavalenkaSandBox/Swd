#pragma once

#include "CoreMinimal.h"
#include "Swd/AI/AIManager.h"
#include "CharacterData.generated.h"

UCLASS()
class SWD_API UCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UClass* AnimInstanceClass = nullptr;

	UPROPERTY(EditAnywhere)
	EFaction Faction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float RunSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float RunSpeedCombat = 350.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float WalkSpeed = 230.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	float WalkSpeedCombat = 180.f;
};
