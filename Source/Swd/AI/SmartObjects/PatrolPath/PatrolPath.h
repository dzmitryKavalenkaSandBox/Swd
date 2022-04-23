// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Swd/AI/SmartObjects/SmartObject.h"
#include "PatrolPath.generated.h"

UCLASS()
class SWD_API APatrolPath : public ASmartObject
{
	GENERATED_BODY()

public:
	APatrolPath();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SplinePath")
	class USplineComponent* Path;

	void GetSplinePoints();

	TArray<FVector> Locations;
};
