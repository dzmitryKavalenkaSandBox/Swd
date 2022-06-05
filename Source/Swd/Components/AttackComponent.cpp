#include "AttackComponent.h"

#include "DamageInflictorComponent.h"
#include "EquipmentComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Modular/Stamina/StaminaComponent.h"
#include "Swd/Swd.h"
#include "Swd/DataAssets/Attack/AttackData.h"
#include "Swd/Utils/Logger.h"
#include "Swd/Weapons/Sword.h"


UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::PerformAttackAnimation()
{
	if (auto Character = Cast<ASwdCharacter>(GetOwner()))
	{
		if (AttackToPreform)
		{
			AttackToPreform->PerformAttackAnimation(Character);
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
	AttackTraceOnTickAndHandleHitResult();
}

void UAttackComponent::SetAttackToPerform(UAttackData* Attack)
{
	if (Attack)
	{
		AttackToPreform = Attack;
		AttackToPreform->Attacker = Cast<ASwdCharacter>(GetOwner());
	}
}

void UAttackComponent::AttackStart()
{
	ULogger::Log(ELogLevel::INFO, __FUNCTION__);
	SetComponentTickEnabled(true);
}

void UAttackComponent::AttackEnd()
{
	SetComponentTickEnabled(false);
	GetCharacter()->StaminaComponent->DrainStamina(GetCurrentAttack()->AttackStaminaFactor);
}

UAttackData* UAttackComponent::GetCurrentAttack()
{
	return AttackToPreform;
}

ASwdCharacter* UAttackComponent::GetCharacter()
{
	return Cast<ASwdCharacter>(GetOwner());
}

FHitResult UAttackComponent::CastAttackTrace(FVector TraceStart, FVector TraceEnd,
                                             FCollisionQueryParams CollisionParams)
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
				float DamageToInflict = BaseDamage * Attack->AttackDamageFactor;
				GetCharacter()->DamageInflictorComponent->InflictDamage(HitResult.GetActor(), DamageToInflict,
				                                                        HitResult);
			}
			else ULogger::Log(ELogLevel::ERROR, TEXT("Trying inflict damage on not set attack"));
		}
	}
}

void UAttackComponent::AttackTraceOnTickAndHandleHitResult()
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
					FHitResult HitResult = CastAttackTrace(TraceStart, TraceEnd, CollisionQueryParams);
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
				FHitResult HitResult = CastAttackTrace(TraceStart, TraceEnd, FCollisionQueryParams());
				HandleTraceHit(HitResult, Character->KickBaseDamage);
				break;
			}
		case EAttackSource::RIGHT_LEG:
			{
				FVector TraceStart = Character->GetActorLocation();
				FVector TraceEnd = Character->RightLegEndTrace->GetComponentLocation();
				FHitResult HitResult = CastAttackTrace(TraceStart, TraceEnd, FCollisionQueryParams());
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
					FHitResult HitResult = CastAttackTrace(TraceStart, TraceEnd, CollisionQueryParams);
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
		                      AttackToPreform->AnimSectionName + FString("' has non AttackSource set"));
			break;
		}
	}
}
