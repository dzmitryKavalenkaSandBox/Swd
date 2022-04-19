#include "Sword.h"

#include "Components/BoxComponent.h"
#include "Swd/Swd.h"
#include "Swd/Components/AttackComponent.h"
#include "Swd/Components/DamageInflictorComponent.h"
#include "Swd/Utils/Logger.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = false;

	PommelCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Pommel Collision Box"));
	PommelCollisionBox->SetupAttachment(WeaponSkeletalMesh);
	PommelCollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);
}

void ASword::OnPommelCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetCharacter())
	{
		if (auto Attack = GetCharacter()->AttackComponent->GetCurrentAttack())
		{
			float DamageToInflict = PommelBaseDamage * Attack->GetAttackDamageFactor();
			GetCharacter()->DamageInflictorComponent->InflictDamage(OtherActor, DamageToInflict);
		}
		else ULogger::Log(ELogLevel::ERROR, TEXT("Trying inflict damage on not set attack"));
	}
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	PommelCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnPommelCollisionBoxOverlapBegin);
}
