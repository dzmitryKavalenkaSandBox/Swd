#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "T_SetAnimationState.generated.h"

enum class EAnimationState : uint8;
/**
 * 
 */
UCLASS()
class SWD_API UT_SetAnimationState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UT_SetAnimationState(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	TMap<EAnimationState, bool> States;
};
