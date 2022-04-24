// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "T_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SWD_API UT_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UT_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
