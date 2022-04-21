#include "LockOnTargetComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


ULockOnTargetComponent::ULockOnTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockOnTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void ULockOnTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRotateOnTick && CurrentTarget)
	{
		FVector LocationOfTarget;
		RotateCharacterToTarget(LocationOfTarget);
		RotateCameraToTarget(LocationOfTarget);
	}
}

void ULockOnTargetComponent::RotateCharacterToTarget(FVector& LocationOfTarget)
{
	if (Owner)
	{
		LocationOfTarget = CurrentTarget->GetActorLocation();
		FVector SelfLocation = Owner->GetActorLocation();
		FRotator SelfRotation = Owner->GetActorRotation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SelfLocation, LocationOfTarget);
		Owner->SetActorRotation(FRotator(SelfRotation.Pitch, LookAtRotation.Yaw, SelfRotation.Roll));
	}
}

void ULockOnTargetComponent::RotateCameraToTarget(FVector LocationOfTarget)
{
	if (Owner && SpringArm)
	{
		FRotator ControlRotation = Owner->GetController()->GetControlRotation();
		FVector CameraWorldLocation = SpringArm->GetComponentLocation();
		FRotator CameraLookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			CameraWorldLocation, LocationOfTarget);
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(ControlRotation, CameraLookAtRotation,
		                                                        GetWorld()->DeltaTimeSeconds, 10.f);
		Owner->GetController()->SetControlRotation(
			FRotator(ControlRotation.Pitch, InterpRotation.Yaw, ControlRotation.Roll)
		);
	}
}

void ULockOnTargetComponent::SetUpComponent(USpringArmComponent* SpringArmComponent, bool ShouldRotateOnTick)
{
	SpringArm = SpringArmComponent;
	bRotateOnTick = ShouldRotateOnTick;
}

void ULockOnTargetComponent::SetTargetToLockOn(AActor* Target)
{
	CurrentTarget = Target;
}
