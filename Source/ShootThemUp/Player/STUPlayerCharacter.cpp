// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "../Components/STUCharacterMovementComponent.h"
#include "../Components/STUHealthComponent.h"
#include "../Components/STUWeaponComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraComponentFP = CreateDefaultSubobject<UCameraComponent>("CameraComponentFP");
	CameraComponentFP->SetupAttachment(GetMesh(), "HeadSocket");
}

void ASTUPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent->SetActive(true);
	CameraComponentFP->SetActive(false);
}

void ASTUPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookAround", this, &ASTUPlayerCharacter::LookAround);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::StopRunning);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASTUPlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASTUPlayerCharacter::StopFire);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);

	PlayerInputComponent->BindAction("ToggleView", IE_Pressed, this, &ASTUPlayerCharacter::ToggleView);
}

void ASTUPlayerCharacter::OnDeath()
{
	Super::OnDeath();

	if (Controller)
		Controller->ChangeState(NAME_Spectating);
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
	MovementInput.X = Amount;
	AddMovementInput(GetActorForwardVector(), Amount);

	if (CharacterMovementComponent->IsRunning() && WeaponComponent->IsFiring())
		WeaponComponent->StopFire();
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
	MovementInput.Y = Amount;
	AddMovementInput(GetActorRightVector(), Amount);

	if (CharacterMovementComponent->IsRunning() && WeaponComponent->IsFiring())
		WeaponComponent->StopFire();
}

void ASTUPlayerCharacter::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ASTUPlayerCharacter::LookAround(float Amount)
{
	AddControllerYawInput(Amount);
}

void ASTUPlayerCharacter::StartRunning()
{
	CharacterMovementComponent->StartRunning();

	if (CharacterMovementComponent->IsRunning())
		WeaponComponent->StopFire();
}

void ASTUPlayerCharacter::StopRunning()
{
	CharacterMovementComponent->StopRunning();
}

void ASTUPlayerCharacter::StartFire()
{
	if (!CharacterMovementComponent->IsRunning())
		WeaponComponent->StartFire();
}

void ASTUPlayerCharacter::StopFire()
{
	WeaponComponent->StopFire();
}

void ASTUPlayerCharacter::ToggleView()
{
	CameraComponent->ToggleActive();
	CameraComponentFP->ToggleActive();

	if (CameraComponentFP->IsActive())
	{
		GetMesh()->HideBoneByName("b_head", EPhysBodyOp::PBO_None);
	}
	else
	{
		GetMesh()->UnHideBoneByName("b_head");
	}
}
