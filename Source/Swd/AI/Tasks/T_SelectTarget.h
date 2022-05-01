// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "UObject/Object.h"
#include "T_SelectTarget.generated.h"

class UEnvQuery;
/**
 * 
 */
UCLASS()
class SWD_API UT_SelectTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UT_SelectTarget(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Blackboard")
	UEnvQuery* EnemySeekerQuery; // set the query in editor

	FEnvQueryRequest EnemySeekerQueryRequest;

	// The function that gets called when querry finished
	void EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	class AAIControllerBase* AIController;

	class ASwdCharacter* BestTarget;
	
};
