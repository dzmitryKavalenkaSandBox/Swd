#include "AttackComponent.h"

#include "DamageInflictorComponent.h"
#include "EquipmentComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Modular/Stamina/StaminaComponent.h"
#include "Swd/Swd.h"
#include "Swd/Utils/Logger.h"
#include "Swd/Weapons/Sword.h"


UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::Attack()
{
	if (auto Character = Cast<ASwdCharacter>(GetOwner()))
	{
		if (AttackToPreform)
		{
			AttackToPreform->PerformAttack(Character);
		}
	}
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DoAttackTrace();
}

void UAttackComponent::SetAttackToPerform(TSubclassOf<UAttackBase> Attack)
{
	if (Attack)
	{
		AttackToPreform = Attack.GetDefaultObject();
		AttackToPreform->Attacker = Cast<ASwdCharacter>(GetOwner());
	}
}

void UAttackComponent::AttackStart()
{
	ULogger::Log(ELogLevel::INFO, __FUNCTION__);
	SetComponentTickEnabled(true);
	SwitchCollisionProfile(CollisionProfile::Weapon);

	// if (auto CollisionBox = GetAttackSourceCollisionBox())
	// {
	// 	CollisionBox->SetNotifyRigidBodyCollision(true);
	// 	CollisionBox->SetGenerateOverlapEvents(true);
	// }
}

void UAttackComponent::AttackEnd()
{
	SetComponentTickEnabled(false);

	// SwitchCollisionProfile(CollisionProfile::NoCollision);
	// if (auto CollisionBox = GetAttackSourceCollisionBox())
	// {
	// 	CollisionBox->SetNotifyRigidBodyCollision(false);
	// 	CollisionBox->SetGenerateOverlapEvents(false);
	// }
	GetCharacter()->StaminaComponent->DrainStamina(GetCurrentAttack()->GetAttackStaminaFactor());
}

void UAttackComponent::SwitchCollisionProfile(FName CollisionProfileName)
{
	if (auto CollisionBox = GetAttackSourceCollisionBox())
	{
		CollisionBox->SetCollisionProfileName(CollisionProfileName);
	}
}

UBoxComponent* UAttackComponent::GetAttackSourceCollisionBox()
{
	auto Character = GetCharacter();
	if (Character && AttackToPreform)
	{
		switch (AttackToPreform->AttackSource)
		{
		case EAttackSource::LEFT_LEG:
			{
				return Character->LeftLegCollisionBox;
			}
		case EAttackSource::RIGHT_LEG:
			{
				return Character->RightLegCollisionBox;
			}
		case EAttackSource::BLADE:
			{
				if (Character->EquipmentComponent->WeaponInHands)
				{
					// return Character->EquipmentComponent->WeaponInHands->CollisionBox;
				}
				ULogger::Log(ELogLevel::ERROR,
				             TEXT("Trying to perfrom attach with weapon having no Weapon in Hands"));
				return nullptr;
			}
		case EAttackSource::POMMEL:
			{
				if (auto Sword = Cast<ASword>(Character->EquipmentComponent->WeaponInHands))
				{
					return Sword->PommelCollisionBox;
				}
				ULogger::Log(ELogLevel::ERROR,
				             TEXT("Trying to perfrom attach with sword having no Sword in Hands"));
				return nullptr;
			}
		default: ULogger::Log(ELogLevel::ERROR, FString("Attack '") +
		                      AttackToPreform->AttackName() + FString("' has non AttackSource set"));
			break;
		}
	}
	return nullptr;
}

UAttackBase* UAttackComponent::GetCurrentAttack()
{
	return AttackToPreform;
}

ASwdCharacter* UAttackComponent::GetCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}

FHitResult UAttackComponent::SwingForAttack(FVector TraceStart, FVector TraceEnd, FCollisionQueryParams CollisionParams)
{
	FHitResult OutHit;
	CollisionParams.AddIgnoredActor(GetOwner());
	// CollisionParams.AddIgnoredComponent(WeaponSkeletalMesh);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		CollisionParams
	);
	DrawDebugTraceLine(TraceStart, TraceEnd);
	return OutHit;
}

void UAttackComponent::DrawDebugTraceLine(FVector TraceStart, FVector TraceEnd)
{
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		TraceEnd,
		FColor::Red,
		false, 1.f, 0,
		2.f
	);
}

void UAttackComponent::HandleTraceHit(FHitResult HitResult, float BaseDamage)
{
	ULogger::Log(ELogLevel::INFO, FString("Hit Bone: ") + HitResult.BoneName.ToString());
	if (HitResult.BoneName != FName("None"))
	{
		SetComponentTickEnabled(false);

		if (HitResult.GetActor() != GetCharacter())
		{
			if (auto Attack = GetCharacter()->AttackComponent->GetCurrentAttack())
			{
				float DamageToInflict = BaseDamage * Attack->GetAttackDamageFactor();
				GetCharacter()->DamageInflictorComponent->InflictDamage(HitResult.GetActor(), DamageToInflict,
				                                                        HitResult);
			}
			else ULogger::Log(ELogLevel::ERROR, TEXT("Trying inflict damage on not set attack"));
		}
	}
}

void UAttackComponent::DoAttackTrace()
{
	auto Character = GetCharacter();
	if (Character && AttackToPreform)
	{
		switch (AttackToPreform->AttackSource)
		{
		case EAttackSource::BLADE:
			{
				if (auto Weapon = Character->EquipmentComponent->WeaponInHands)
				{
					FVector TraceStart = Weapon->BladeDamagePointStart->GetComponentLocation();
					FVector TraceEnd = Weapon->BladeDamagePointEnd->GetComponentLocation();
					FCollisionQueryParams CollisionQueryParams;
					CollisionQueryParams.AddIgnoredComponent(Weapon->WeaponSkeletalMesh);
					FHitResult HitResult = SwingForAttack(TraceStart, TraceEnd, CollisionQueryParams);
					HandleTraceHit(HitResult, Weapon->GetWeaponBaseDamage());
				}
				else
				{
					ULogger::LogError(
						TEXT("[NO WEAPON IN HANDS] Trying to perfrom attack with weapon having no Weapon in Hands")
					);
				}
				break;
			}
		case EAttackSource::LEFT_LEG:
			{
				FVector TraceStart = Character->GetActorLocation();
				FVector TraceEnd = Character->LeftLegEndTrace->GetComponentLocation();
				FHitResult HitResult = SwingForAttack(TraceStart, TraceEnd, FCollisionQueryParams());
				HandleTraceHit(HitResult, Character->KickBaseDamage);
				break;
			}
		case EAttackSource::RIGHT_LEG:
			{
				FVector TraceStart = Character->GetActorLocation();
				FVector TraceEnd = Character->RightLegEndTrace->GetComponentLocation();
				FHitResult HitResult = SwingForAttack(TraceStart, TraceEnd, FCollisionQueryParams());
				HandleTraceHit(HitResult, Character->KickBaseDamage);
				break;
			}
		case EAttackSource::POMMEL:
			{
				if (auto Sword = Cast<ASword>(Character->EquipmentComponent->WeaponInHands))
				{
					FVector TraceStart = Character->GetActorLocation();
					FVector TraceEnd = Sword->PommelAttackTraceEnd->GetComponentLocation();
					FCollisionQueryParams CollisionQueryParams;
					CollisionQueryParams.AddIgnoredComponent(Sword->WeaponSkeletalMesh);
					FHitResult HitResult = SwingForAttack(TraceStart, TraceEnd, CollisionQueryParams);
					HandleTraceHit(HitResult, Sword->PommelBaseDamage);
				}
				else
				{
					ULogger::LogError(
						TEXT("[NO WEAPON IN HANDS] Trying to perfrom attack with weapon having no Weapon in Hands")
					);
				}
				break;
			}
		default: ULogger::Log(ELogLevel::ERROR, FString("Attack '") +
		                      AttackToPreform->AttackName() + FString("' has non AttackSource set"));
			break;
		}
	}
}
