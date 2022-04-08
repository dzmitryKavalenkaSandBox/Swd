#include "WeaponBase.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Swd/Swd.h"

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
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(WeaponSkeletalMesh);
	SphereCollision->SetCollisionProfileName("OverlapAllDynamics");
	SphereCollision->SetGenerateOverlapEvents(true);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(WeaponSkeletalMesh);
	CollisionBox->SetCollisionProfileName(CollisionProfile::NoCollision);

	WeaponStart = CreateDefaultSubobject<USceneComponent>(TEXT("Sword Start"));
	WeaponStart->SetupAttachment(WeaponSkeletalMesh);
	WeaponEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Sword End"));
	WeaponEnd->SetupAttachment(WeaponSkeletalMesh);
}

float AWeaponBase::GetWeaponBaseDamage()
{
	return BaseDamage;
}

USceneComponent* AWeaponBase::GetWeaponStart()
{
	return WeaponStart;
}

USceneComponent* AWeaponBase::GetWeaponEnd()
{
	return WeaponEnd;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}