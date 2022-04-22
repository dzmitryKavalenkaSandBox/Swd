// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Fatiguable.generated.h"

UINTERFACE()
class UFatiguable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWD_API IFatiguable
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void UpdateStaminaOnWidget() const;
};
