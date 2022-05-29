#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "T_SetRandomMoveToLocationAroundTarget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SWD_API UT_SetRandomMoveToLocationAroundTarget  : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UT_SetRandomMoveToLocationAroundTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float RadiusAroundPLayer = 300.f;
};
