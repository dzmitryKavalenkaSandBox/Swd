// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwdCharacter.h"
#include "UObject/Object.h"
#include "AICharacterBase.generated.h"

/**
 * 
 */
class UWidgetComponent;
UCLASS()
class SWD_API AAICharacterBase : public ASwdCharacter
{
	GENERATED_BODY()

	AAICharacterBase();

	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UWidgetComponent* HealthStaminaWidgetComponent;

	void SetUpHealthStaminaWidget();
	void UpdateHealthStaminaWidget() const;
};
