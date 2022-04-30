// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "T_PrepareForCombat.generated.h"

/**
 * 
 */
UCLASS()
class SWD_API UT_PrepareForCombat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UT_PrepareForCombat();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
