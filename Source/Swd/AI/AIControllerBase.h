// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

UCLASS()
class SWD_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIControllerBase();

	void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BTC;

	UPROPERTY(transient)
	UBlackboardComponent* BBC;
	
	UPROPERTY(BlueprintReadWrite)
	class AAICharacterBase* Agent;

	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	uint8 ContactKeyId;
};
