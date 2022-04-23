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
	TCHAR const* const MoveToLocation = TEXT("MoveToLocation");
	TCHAR const* const Contact = TEXT("Contact");
}

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle              UMETA(DisplayName = "Idle"),
	ManDown           UMETA(DisplayName = "Man Down"),
	Investigate       UMETA(DisplayName = "Investigate"),
	LostEnemy         UMETA(DisplayName = "Lost Enemy"),
	Search            UMETA(DisplayName = "Search"),
	Attack            UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Enemy            UMETA(DisplayName = "Enemy"),
	Friendly         UMETA(DisplayName = "Friendly"),
	Neutral          UMETA(DisplayName = "Neutral")
};
