// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyQueryContext.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Swd.h"
#include "Swd/AI/AIControllerBase.h"
#include "Swd/Utils/Logger.h"

void UEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AAIControllerBase* Cntrlr = Cast<AAIControllerBase>(
		Cast<AActor>(QueryInstance.Owner.Get())->GetInstigatorController());
	if (Cntrlr)
	{
		AActor* Target = Cast<AActor>(Cntrlr->BBC->GetValueAsObject(BBKeys::ClosestHostile));
		if (Target)
		{
			// ULogger::Log(ELogLevel::WARNING, FString("Setting Target to ") + Target->GetName());
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Target);
		}
	}
}
