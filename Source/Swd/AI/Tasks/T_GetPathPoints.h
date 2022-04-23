// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UObject/Object.h"
#include "T_GetPathPoints.generated.h"

/**
 * 
 */
UCLASS()
class SWD_API UT_GetPathPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UT_GetPathPoints(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 Index = 0;
};