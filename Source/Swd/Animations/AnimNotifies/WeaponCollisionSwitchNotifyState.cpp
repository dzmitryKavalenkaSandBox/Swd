#include "WeaponCollisionSwitchNotifyState.h"

#include "Swd/Character/SwdCharacter.h"
#include "Swd/Components/AttackComponent.h"

void UWeaponCollisionSwitchNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner()))
	{
		Character->AttackComponent->AttackStart();
	}
}

void UWeaponCollisionSwitchNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (auto Character = Cast<ASwdCharacter>(MeshComp->GetOwner()))
	{
		Character->AttackComponent->AttackEnd();
	}
}
