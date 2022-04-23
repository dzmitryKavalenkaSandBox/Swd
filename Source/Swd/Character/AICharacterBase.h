// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwdCharacter.h"
#include "AICharacterBase.generated.h"

/**
 * 
 */
class UWidgetComponent;
class USphereComponent;
UCLASS()
class SWD_API AAICharacterBase : public ASwdCharacter
{
	GENERATED_BODY()

	AAICharacterBase();

	virtual void BeginPlay() override;
public:
	virtual void UpdateCurrentHealth(float NewValue) override;

	virtual void UpdateHealthOnWidget() const override;

	virtual void UpdateStaminaOnWidget() const override;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* TreeAsset;


protected:
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	UWidgetComponent* HealthStaminaWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	USphereComponent* SphereAroundAI;

	void SetUpHealthStaminaWidget();

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
