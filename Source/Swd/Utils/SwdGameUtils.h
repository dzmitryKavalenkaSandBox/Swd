// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Swd/Character/SwdCharacter.h"
#include "SwdGameUtils.generated.h"

enum class EAIState : uint8;
struct FPlayerAnimMontage;
/**
 * 
 */
UCLASS()
class SWD_API USwdGameUtils : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	static float PlayAnimMontage(ASwdCharacter* AnimationOwner, FPlayerAnimMontage* AnimMontage,
	                             float InPlayRate = 1.f,
	                             FName StartSectionName = FName("Default"));

	static AActor* GetClosestActor(FVector SourceLocation, TArray<AActor*> Actors);

	// static UAnimMontage* GetRandomMontage(class UDataTable* DataTable, FName RowName, FString AnimAction);
};
