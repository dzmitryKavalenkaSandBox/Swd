// Fill out your copyright notice in the Description page of Project Settings.


#include "T_GetPathPoints.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Swd/Swd.h"
#include "Swd/AI/SmartObjects/PatrolPath/PatrolPath.h"
#include "Swd/Character/AICharacterBase.h"

UT_GetPathPoints::UT_GetPathPoints(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Path Points";
}

EBTNodeResult::Type UT_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController || !BBComponent)
	{
		return EBTNodeResult::Failed;
	}

	AAICharacterBase* Chr = Cast<AAICharacterBase>(AIController->GetPawn());
	if (!Chr)
	{
		return EBTNodeResult::Failed;
	}

	APatrolPath* PathRef = Cast<APatrolPath>(Chr->SmartObject);
	if (!PathRef || PathRef->Locations.Num() < 1)
	{
		return EBTNodeResult::Succeeded;
	}

	BBComponent->SetValueAsVector(BBKeys::MoveToLocation, PathRef->Locations[Index]);

	if (Index < PathRef->Locations.Num() - 1)
	{
		Index++;
		return EBTNodeResult::Succeeded;
	}
	Index = 0;
	return EBTNodeResult::Succeeded;
}
