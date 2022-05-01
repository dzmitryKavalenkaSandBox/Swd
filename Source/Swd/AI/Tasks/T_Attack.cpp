// Fill out your copyright notice in the Description page of Project Settings.


#include "T_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Swd.h"
#include "Swd/Utils/Logger.h"

UT_Attack::UT_Attack()
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto Hostile = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::BestTarget))
	{
		ULogger::Log(ELogLevel::WARNING, FString("Attacking Target: ") + Hostile->GetName());
	}
	else
	{
		ULogger::Log(ELogLevel::WARNING, FString("Attacking With null target actor"));
	}
	return EBTNodeResult::Succeeded;
}
