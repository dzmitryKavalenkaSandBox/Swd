#include "T_SelectTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Character/AICharacterBase.h"


UT_SelectTarget::UT_SelectTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Select Target");
}

EBTNodeResult::Type UT_SelectTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	if (AIController && EnemySeekerQuery)
	{
		EnemySeekerQueryRequest = FEnvQueryRequest(EnemySeekerQuery, AIController->Agent);
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this,
		                                &UT_SelectTarget::EnemySeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UT_SelectTarget::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	BestTarget = nullptr;

	float CurrentBestScore = 0.f;
	int32 Index = 0;
	TArray<AActor*> AllDetectedActors;
	Result->GetAllAsActors(AllDetectedActors);

	for (auto& DetectedActor : AllDetectedActors)
	{
		ASwdCharacter* Chr = Cast<ASwdCharacter>(DetectedActor);
		if (Chr && AIController->Agent->IsHostile(Chr) /*&& !Chr->Dead*/)
		{
			if (Result->GetItemScore(Index) > CurrentBestScore && Result->GetItemScore(Index) > 0.f)
			{
				BestTarget = Chr;
				CurrentBestScore = Result->GetItemScore(Index);
			}

			// Increment Index
			Index++;
		}
	}

	if (BestTarget)
	{
		// ULogger::Log(ELogLevel::WARNING, FString("Setting target from SelectTargetTask to ") + BestTarget->GetName());
		AIController->BBC->SetValueAsObject(GetSelectedBlackboardKey(), BestTarget);
	}
}
