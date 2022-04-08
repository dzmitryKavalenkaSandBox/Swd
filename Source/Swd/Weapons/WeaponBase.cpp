#include "WeaponBase.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Swd/Swd.h"
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
}

float AWeaponBase::GetWeaponBaseDamage()
{
	return BaseDamage;
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
	ULogger::Log(ELogLevel::WARNING, __FUNCTION__);
}

void AWeaponBase::OnInteractionSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ULogger::Log(ELogLevel::WARNING, __FUNCTION__);
}

void AWeaponBase::OnCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	ULogger::Log(ELogLevel::WARNING, __FUNCTION__);
}
