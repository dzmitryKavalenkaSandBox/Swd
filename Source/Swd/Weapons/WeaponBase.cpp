#include "WeaponBase.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Swd/Swd.h"
#include "Swd/Components/AttackComponent.h"
#include "Swd/Components/DamageInflictorComponent.h"
#include "Swd/Utils/Logger.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	WeaponSkeletalMesh->SetupAttachment(RootComponent);
	/** Block all, Ignore Pawn, Visibility and Camera
	 *  WeaponSkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	 * 	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	 * 	WeaponSkeletalMesh->SetSimulatePhysics(true);
	 */
	WeaponSkeletalMesh->SetCollisionProfileName(CollisionProfile::NoCollision);
	WeaponSkeletalMesh->SetSimulatePhysics(true);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	InteractionSphere->SetupAttachment(WeaponSkeletalMesh);
	InteractionSphere->SetCollisionProfileName("OverlapAllDynamics");
	InteractionSphere->SetGenerateOverlapEvents(true);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(WeaponSkeletalMesh);
	CollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);

	DrawWeaponSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Draw Weapon Audio cpmponent"));
	SheathWeaponSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sheath Weapon Audio cpmponent"));
}

float AWeaponBase::GetWeaponBaseDamage()
{
	return BaseDamage;
}

void AWeaponBase::SetOwnerCharacter(ASwdCharacter* NewOwner)
{
	SetOwner(NewOwner);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnInteractionSphereOverlapBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnInteractionSphereOverlapEnd);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnCollisionBoxOverlapBegin);

	CollisionBox->AttachToComponent(
		WeaponSkeletalMesh,
		FAttachmentTransformRules(
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			false
		),
		BoneSockets::CollisionBoxSocket
	);
}

void AWeaponBase::OnInteractionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	// ULogger::Log(ELogLevel::WARNING, __FUNCTION__);
}

void AWeaponBase::OnInteractionSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ULogger::Log(ELogLevel::WARNING, __FUNCTION__);
}

void AWeaponBase::OnCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (OtherActor != GetCharacter())
	{
		if (auto Attack = GetCharacter()->AttackComponent->GetCurrentAttack())
		{
			float DamageToInflict = Attack->GetAttackDamageFactor() + GetWeaponBaseDamage();
			GetCharacter()->DamageInflictorComponent->InflictDamage(OtherActor, DamageToInflict);
		}
		else ULogger::Log(ELogLevel::ERROR, TEXT("Trying inflict damage on not set attack"));
	}
}

ASwdCharacter* AWeaponBase::GetCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}

void AWeaponBase::PlayDrawWeaponSound()
{
	if (DrawWeaponSoundComponent && !DrawWeaponSoundComponent->IsPlaying())
	{
		// default pitch value is 1.f
		DrawWeaponSoundComponent->SetPitchMultiplier(FMath::RandRange(1.0f, 1.3f));
		DrawWeaponSoundComponent->SetSound(DrawWeaponSoundCue);
		DrawWeaponSoundComponent->SetVolumeMultiplier(0.3);

		DrawWeaponSoundComponent->Play(0.f);
	}
}

void AWeaponBase::PlaySheathWeaponSound()
{
	if (SheathWeaponSoundComponent && !SheathWeaponSoundComponent->IsPlaying())
	{
		// default pitch value is 1.f
		SheathWeaponSoundComponent->SetPitchMultiplier(FMath::RandRange(1.0f, 1.3f));
		SheathWeaponSoundComponent->SetSound(SheathWeaponSoundCue);
		SheathWeaponSoundComponent->SetVolumeMultiplier(0.3);

		SheathWeaponSoundComponent->Play(0.f);
	}
}
