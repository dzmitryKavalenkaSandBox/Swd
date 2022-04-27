#include "T_SelectTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Utils/Logger.h"


UT_SelectTarget::UT_SelectTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Select Target");
}

EBTNodeResult::Type UT_SelectTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cntrlr = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());

	if (Cntrlr && EnemySeekerQuery)
	{
		EnemySeekerQueryRequest = FEnvQueryRequest(EnemySeekerQuery, Cntrlr->Agent);
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this,
		                                &UT_SelectTarget::EnemySeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UT_SelectTarget::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	BestTarget = nullptr;
	// ULogger::Log(ELogLevel::WARNING, FString("Target actor set to NULL"));
	// Cntrlr->BBC->SetValueAsObject(BBKeys::TargetActor, nullptr);

	float CurrentBestScore = 0.f;
	int32 Index = 0;
	TArray<AActor*> AllDetectedActors;
	Result->GetAllAsActors(AllDetectedActors);

	for (auto& DetectedActor : AllDetectedActors)
	{
		ASwdCharacter* Chr = Cast<ASwdCharacter>(DetectedActor);
		if (Chr && Cntrlr->Agent->IsHostile(Chr) /*&& !Chr->Dead*/)
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
		Cntrlr->BBC->SetValueAsObject(GetSelectedBlackboardKey(), BestTarget);
	}
}
