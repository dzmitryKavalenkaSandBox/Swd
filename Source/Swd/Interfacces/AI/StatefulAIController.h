#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatefulAIController.generated.h"

class AAICharacterBase;
class AAICharacter;
class UBlackboardComponent;
UENUM(BlueprintType)
	enum class EAIState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	OnDuty UMETA(DisplayName = "OnDuty"),
	Alerted UMETA(DisplayName = "Alerted"),
	ManDown UMETA(DisplayName = "Man Down"),
	LostEnemy UMETA(DisplayName = "Lost Enemy"),
	Search UMETA(DisplayName = "Search"),
	Attack UMETA(DisplayName = "Attack")
};


UINTERFACE()
class UStatefulAIController : public UInterface
{
	GENERATED_BODY()
};

class SWD_API IStatefulAIController
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void UpdateAIState(EAIState State);

	UFUNCTION()
	virtual UBlackboardComponent* GetBBC();

	UFUNCTION()
	virtual bool IsStateEquals(EAIState State);
};
