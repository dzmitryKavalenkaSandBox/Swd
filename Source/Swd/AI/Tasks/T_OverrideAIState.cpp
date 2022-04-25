#include "T_OverrideAIState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Swd/Swd.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Utils/SwdGameUtils.h"

UT_OverrideAIState::UT_OverrideAIState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = TEXT("AIState Override");
}

EBTNodeResult::Type UT_OverrideAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (!MyController || !MyController->AIManager)
	{
		return EBTNodeResult::Failed;
	}

	if (RunMode == ERunMode::ThisAgent)
	{
		MyController->UpdateAIState(DesiredState);
	}
	else
	{
		MyController->AIManager->NotifyAllAgentsAIState(DesiredState);
	}
	return EBTNodeResult::Succeeded;
}
