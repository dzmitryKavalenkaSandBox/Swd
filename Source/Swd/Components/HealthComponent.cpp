#include "HealthComponent.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Swd/Character/SwdCharacter.h"
#include "Swd/Interfacces/Killable.h"
#include "Swd/Utils/Logger.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// static ConstructorHelpers::FObjectFinder<USoundCue> HitSoundCueObject(
	// 	TEXT(""));
	// if (HitSoundCueObject.Succeeded())
	// {
	// 	HitSoundCue = HitSoundCueObject.Object;
	// }
		HitSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Hit Sound Component"));
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	if (HitSoundComponent && HitSoundCue)
	{
		HitSoundComponent->SetSound(HitSoundCue);
	}
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                   AController* Instigator, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHealthOnWidgets(DamagedActor);
	HitSoundComponent->Play(0.f);
	if (Health <= 0.f)
	{
		auto DeadActor = Cast<IKillable>(DamagedActor);
		if (DeadActor)
		{
			DeadActor->HandleDeathBehavior();
		}
	}
	if(auto HitCharacter = Cast<ASwdCharacter>(DamagedActor))
	{
		ULogger::Log(ELogLevel::WARNING, FString("Health of hit actor: ") + FString::FromInt(HitCharacter->HealthComponent->Health));
	}
}

void UHealthComponent::UpdateHealthOnWidgets(AActor* DamagedActor)
{
	// if (auto AICharacter = Cast<AAICharacter>(DamagedActor))
	// {
	// 	AICharacter->UpdateHealthStaminaWidget();
	// }
	//
	// if (auto PlayerCharacter = Cast<APlayerCharacter>(DamagedActor))
	// {
	// 	PlayerCharacter->UpdateHUDWidget();
	// }
}
