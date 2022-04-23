#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmartObject.generated.h"

UCLASS()
class SWD_API ASmartObject : public AActor
{
	GENERATED_BODY()

public:
	ASmartObject();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* FacingDirection;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* SubTree;
};
