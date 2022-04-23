#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Swd/Components/Modular/LockOnTargetModule/LockOnTargetComponent.h"
#include "Swd/Components/Modular/Stamina/StaminaComponent.h"
#include "Swd/UI/HealthStaminaWidget.h"
#include "Swd/UI/HUDWidget.h"
#include "Swd/Utils/Logger.h"


APlayerCharacter::APlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	SetUpHUDWidget();
	SetUpHealthStaminaWidget();

	LockOnTargetComponent = CreateDefaultSubobject<ULockOnTargetComponent>(TEXT("Lock On Target Component"));
	LockOnTargetComponent->SetUpComponent(CameraBoom, true);

	ClosestActorFinderComponent = CreateDefaultSubobject<UClosestActorFinderComponent>(
		TEXT("Closest Actor Finder Component"));
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (auto Widget = Cast<UHUDWidget>(HUDWidgetComponent->GetWidget()))
	{
		Widget->AddToViewport();
	}
	UpdateHealthOnWidget();
	UpdateStaminaOnWidget();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &APlayerCharacter::SwitchTargetToLockOn);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &APlayerCharacter::LookUpAtRate);

	// handle touch devices
	// PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerCharacter::OnTouchBegin);
	// PlayerInputComponent->BindTouch(IE_Released, this, &APlayerCharacter::OnTouchEnd);

	PlayerInputComponent->BindAction(TEXT("EquipSheath"), IE_Pressed, this, &APlayerCharacter::EquipSheathWeapon);
}

void APlayerCharacter::SetUpHUDWidget()
{
	HUDWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HUD Widget Component"));
	HUDWidgetComponent->SetupAttachment(RootComponent);
	HUDWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
}

void APlayerCharacter::SetUpHealthStaminaWidget()
{
	HealthStaminaWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Stamina Widget Component"));
	HealthStaminaWidgetComponent->SetupAttachment(RootComponent);
	HealthStaminaWidgetComponent->SetDrawSize(FVector2D(80, 20));
	HealthStaminaWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthStaminaWidgetComponent->SetVisibility(true);
}

void APlayerCharacter::UpdateHealthOnWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->HealthBar->SetPercent(GetCurrentHealth() / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}

void APlayerCharacter::UpdateStaminaOnWidget() const
{
	if (auto Widget = Cast<UHealthStaminaWidget>(HealthStaminaWidgetComponent->GetWidget()))
	{
		Widget->StaminaBar->SetPercent(StaminaComponent->CurrentStamina / 100);
	}
	else
	{
		ULogger::Log(ELogLevel::ERROR, FString("Widget for Stamina/Health not set on " + this->GetName()));
	}
}

void APlayerCharacter::UpdateCurrentHealth(float NewValue)
{
	Super::UpdateCurrentHealth(NewValue);
	UpdateHealthOnWidget();
}

void APlayerCharacter::ManageCombatState(bool bEnableCombat)
{
	Super::ManageCombatState(bEnableCombat);
	if (!bEnableCombat)
	{
		LockOnTargetComponent->SetTargetToLockOn(nullptr);
	}
}

void APlayerCharacter::SwitchTargetToLockOn()
{
	AActor* NewTarget = ClosestActorFinderComponent->GetClosestTarget();
	if (GetIsInCombat() && NewTarget && NewTarget != LockOnTargetComponent->GetCurrentTarget())
	{
		LockOnTargetComponent->SetTargetToLockOn(NewTarget);
	}
	else
	{
		LockOnTargetComponent->SetTargetToLockOn(nullptr);
	}
}
