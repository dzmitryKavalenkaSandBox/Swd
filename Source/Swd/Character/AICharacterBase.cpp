// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterBase.h"

#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Swd/Components/HealthComponent.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/Utils/Logger.h"

AAICharacterBase::AAICharacterBase()
{
	SetUpHealthStaminaWidget();
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UpdateHealthStaminaWidget();
}

void AAICharacterBase::SetUpHealthStaminaWidget()
{
	HealthStaminaWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Stamina Widget Component"));
	HealthStaminaWidgetComponent->SetupAttachment(RootComponent);
	HealthStaminaWidgetComponent->SetDrawSize(FVector2D(50, 20));
	HealthStaminaWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthStaminaWidgetComponent->SetVisibility(true);
}

void AAICharacterBase::UpdateHealthStaminaWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->HealthBar->SetPercent(HealthComponent->Health / 100);
		// Widget->StaminaBar->SetPercent(StaminaComponent->CurrentStamina / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}