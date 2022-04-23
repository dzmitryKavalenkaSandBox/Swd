// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Swd/Character/SwdCharacter.h"
#include "UObject/Interface.h"
#include "DataTableAnimated.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAnimMontage : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AnimSectionCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
};

// This class does not need to be modified.
UINTERFACE()
class UDataTableAnimated : public UInterface
{
	GENERATED_BODY()
};

class SWD_API IDataTableAnimated
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void PlayDataTableAnimation(ASwdCharacter* Character, UDataTable* DataTable, bool HasBlendedAnim,
	                                    FString AnimActionName);
};
