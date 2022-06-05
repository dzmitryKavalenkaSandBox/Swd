#include "Sword.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Swd/Swd.h"
#include "Swd/Components/AttackComponent.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = false;

	PommelCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Pommel Collision Box"));
	PommelAttackTraceEnd = CreateDefaultSubobject<USphereComponent>(TEXT("Pommel Attack Trace End"));
	PommelCollisionBox->SetupAttachment(WeaponSkeletalMesh);
	PommelAttackTraceEnd->SetupAttachment(WeaponSkeletalMesh);
	PommelCollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);
	PommelAttackTraceEnd->SetRelativeScale3D(FVector(.2, .2, .2));
}
//
// void ASword::OnPommelCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
//                                               const FHitResult& SweepResult)
// {
// 	if (OtherActor != GetCharacter())
// 	{
// 		if (auto Attack = GetCharacter()->AttackComponent->GetCurrentAttack())
// 		{
// 			float DamageToInflict = PommelBaseDamage * Attack->GetAttackDamageFactor();
// 			GetCharacter()->DamageInflictorComponent->InflictDamage(OtherActor, DamageToInflict, SweepResult);
// 		}
// 		else ULogger::Log(ELogLevel::ERROR, TEXT("Trying inflict damage on not set attack"));
// 	}
// }

void ASword::BeginPlay()
{
	Super::BeginPlay();
	// PommelCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnPommelCollisionBoxOverlapBegin);
}
