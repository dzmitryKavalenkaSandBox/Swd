#include "AICharacterBase.h"

#include "PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Swd/Components/Modular/Stamina/StaminaComponent.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/Utils/Logger.h"

AAICharacterBase::AAICharacterBase()
{
	SetUpHealthStaminaWidget();
	SphereAroundAI = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Around AI"));

	SphereAroundAI->SetupAttachment(RootComponent);
	SphereAroundAI->SetCollisionProfileName(FName("OverlapAll"));
	SphereAroundAI->SetGenerateOverlapEvents(true);
	SphereAroundAI->SetSphereRadius(600.f);
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UpdateHealthOnWidget();
	UpdateStaminaOnWidget();
	SphereAroundAI->OnComponentBeginOverlap.AddDynamic(this, &AAICharacterBase::OnSphereOverlapBegin);
	SphereAroundAI->OnComponentEndOverlap.AddDynamic(this, &AAICharacterBase::OnSphereOverlapEnd);
}

void AAICharacterBase::SetUpHealthStaminaWidget()
{
	HealthStaminaWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Stamina Widget Component"));
	HealthStaminaWidgetComponent->SetupAttachment(RootComponent);
	HealthStaminaWidgetComponent->SetDrawSize(FVector2D(80, 20));
	HealthStaminaWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthStaminaWidgetComponent->SetVisibility(false);
}

void AAICharacterBase::UpdateHealthOnWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->HealthBar->SetPercent(GetCurrentHealth() / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}

void AAICharacterBase::UpdateStaminaOnWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->StaminaBar->SetPercent(StaminaComponent->CurrentStamina / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}

void AAICharacterBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		HealthStaminaWidgetComponent->SetVisibility(true);
	}
}

void AAICharacterBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		HealthStaminaWidgetComponent->SetVisibility(false);
	}
}

void AAICharacterBase::UpdateCurrentHealth(float NewValue)
{
	Super::UpdateCurrentHealth(NewValue);
	UpdateHealthOnWidget();
}

