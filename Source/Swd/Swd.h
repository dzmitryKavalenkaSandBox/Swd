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
	FName const HolstedSwordSocket = FName("HolstedSword");
	FName const LeftFootSocket = FName("LeftFootSocket");
	FName const RightFootSocket = FName("RightFootSocket");
	FName const SwordHandleSocket = FName("Sword_Handle");
	FName const CollisionBoxSocket = FName("CollisionBoxSocket");
}