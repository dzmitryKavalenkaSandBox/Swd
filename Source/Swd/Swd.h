// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace CollisionProfile
{
	TCHAR const* const Weapon = TEXT("Weapon");
	TCHAR const* const NoCollision = TEXT("NoCollision");
}

namespace BoneSockets
{
	FName const HipWeaponSocket = FName("HipWeaponSocket");
	FName const LeftFootSocket = FName("LeftFootSocket");
	FName const RightFootSocket = FName("RightFootSocket");
	FName const WeaponHandle = FName("WeaponHandle");
	FName const CollisionBoxSocket = FName("CollisionBoxSocket");
	FName const RightFoodKickSocket = FName("RightFoodKickSocket");
	FName const LeftFoodKickSocket = FName("LeftFoodKickSocket");
}

namespace AM_DataTableReference
{
	TCHAR const* const EquipSheathWeapon = TEXT("DataTable'/Game/Assets/DataTables/DT_EquipSword.DT_EquipSword'");
	TCHAR const* const Attacks = TEXT("");
}

namespace AnimAction
{
	TCHAR const* const Equip = TEXT("Equip");
	TCHAR const* const Sheath = TEXT("Sheath");
}

namespace BBKeys
{
	TCHAR const* const TargetActor = TEXT("TargetActor");
	TCHAR const* const SensedActor = TEXT("SensedActor");
	TCHAR const* const MoveToLocation = TEXT("MoveToLocation");
	TCHAR const* const Contact = TEXT("Contact");
	TCHAR const* const AIState = TEXT("AIState");
	TCHAR const* const LastLocationOfClosestHostile = TEXT("LastLocationOfClosestHostile");
	TCHAR const* const Damaged = TEXT("Damaged");
	TCHAR const* const ClosestHostile = TEXT("ClosestHostile");
	TCHAR const* const CombatState = TEXT("CombatState");
}

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Enemy            UMETA(DisplayName = "Enemy"),
	Friendly         UMETA(DisplayName = "Friendly"),
	Neutral          UMETA(DisplayName = "Neutral")
};


UENUM(BlueprintType)
enum class ECombatRole : uint8
{
	Engager           UMETA(DisplayName = "Engager"),
	Defender          UMETA(DisplayName = "Defender"),
	Sniper            UMETA(DisplayName = "Sniper")
};


UENUM(BlueprintType)
enum class ECombatState : uint8
{
	SoftCover         UMETA(DisplayName = "Soft Cover"),
	ApproachingCover  UMETA(DisplayName = "Approaching Cover"),
	Flank             UMETA(DisplayName = "Flank"),
	HoldCover         UMETA(DisplayName = "HoldCover"),
	RetreatingCover   UMETA(DisplayName = "RetreatingCover"),
	Evade             UMETA(DisplayName = "Evade")
};