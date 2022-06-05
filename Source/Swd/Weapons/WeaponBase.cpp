#include "WeaponBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Swd/Swd.h"
#include "Swd/Character/AICharacterBase.h"
#include "Swd/Components/AttackComponent.h"

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

	BladeDamagePointStart = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Point Start"));
	BladeDamagePointStart->SetupAttachment(WeaponSkeletalMesh);
	BladeDamagePointStart->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	BladeDamagePointEnd = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Point End"));
	BladeDamagePointEnd->SetupAttachment(WeaponSkeletalMesh);
	BladeDamagePointEnd->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));

	// CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	// CollisionBox->SetupAttachment(WeaponSkeletalMesh);
	// CollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);

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
	SetActorTickEnabled(false);
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnInteractionSphereOverlapBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnInteractionSphereOverlapEnd);

	// CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnCollisionBoxOverlapBegin);
	//
	// CollisionBox->AttachToComponent(
	// 	WeaponSkeletalMesh,
	// 	FAttachmentTransformRules(
	// 		EAttachmentRule::KeepWorld,
	// 		EAttachmentRule::KeepWorld,
	// 		EAttachmentRule::KeepWorld,
	// 		false
	// 	),
	// 	BoneSockets::CollisionBoxSocket
	// );
}

void AWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

//
// void AWeaponBase::OnCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
//                                              const FHitResult& SweepResult)
// {
// 	ULogger::Log(ELogLevel::WARNING, FString("Hit Component name: ") + SweepResult.Component->GetName());
// 	if (auto AI = Cast<AAICharacterBase>(OtherActor))
// 	{
// 		ULogger::Log(ELogLevel::WARNING, FString("Hit Bone name: ") + AI->GetMesh()->GetBoneName(OtherBodyIndex).ToString());
// 	}
// 	if (OtherActor != GetCharacter())
// 	{
// 		if (auto Attack = GetCharacter()->AttackComponent->GetCurrentAttack())
// 		{
// 			float DamageToInflict = GetWeaponBaseDamage() * Attack->GetAttackDamageFactor();
// 			GetCharacter()->DamageInflictorComponent->InflictDamage(OtherActor, DamageToInflict, SweepResult);
// 		}
// 		else ULogger::Log(ELogLevel::ERROR, TEXT("Trying inflict damage on not set attack"));
// 	}
// }

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
