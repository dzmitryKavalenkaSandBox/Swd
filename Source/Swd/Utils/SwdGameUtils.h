// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Swd/Character/SwdCharacter.h"
#include "SwdGameUtils.generated.h"

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
};
