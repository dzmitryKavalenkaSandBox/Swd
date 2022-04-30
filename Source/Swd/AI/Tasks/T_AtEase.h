#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "T_AtEase.generated.h"

/**
 * 
 */
UCLASS()
class SWD_API UT_AtEase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UT_AtEase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

