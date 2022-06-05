#include "AttackData.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Utils/Logger.h"
#include "Swd/Utils/SwdGameUtils.h"


void UAttackData::PerformAttackAnimation(ASwdCharacter* Character)
{
	UAnimMontage* MontageToPlay = nullptr;
	if (Character)
	{
		if (Character->GetSpeed() <= 0 || !BlendedAttackMontage)
		{
			Character->bIsAnimationBlended = false;
			MontageToPlay = AttackMontage;
		}
		if (Character->GetSpeed() > 0 && BlendedAttackMontage)
		{
			Character->bIsAnimationBlended = true;
			MontageToPlay = BlendedAttackMontage;
		}
		if (MontageToPlay)
		{
			FString MontageSection;
			if (MontageToPlay->CompositeSections.Num() == 1)
			{
				MontageSection = AnimSectionName;
			}
			else
			{
				int MontageSectionIndex = rand() % MontageToPlay->CompositeSections.Num() + 1;
				MontageSection = AnimSectionName + FString::FromInt(MontageSectionIndex);
			}
			USwdGameUtils::PlayAnimMontage(Character, MontageToPlay, 1.f, FName(*MontageSection));
		}
		else
		{
			ULogger::LogError(TEXT("Attack Montage was not provided for Attack Data asset"));
		}
	}
}
