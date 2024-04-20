// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"

#include "STUCharacterMovementComponent.h"
#include "STUHealthComponent.h"

#include "../Weapon/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged(HealthComponent->GetHealth(), HealthComponent->MaxHealth);

	HealthComponent->OnDeath.AddDynamic(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTUBaseCharacter::OnHealthChanged);
	
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnLand);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookAround", this, &ASTUBaseCharacter::LookAround);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, Cast<USTUCharacterMovementComponent>(GetCharacterMovement()), &USTUCharacterMovementComponent::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, Cast<USTUCharacterMovementComponent>(GetCharacterMovement()), &USTUCharacterMovementComponent::StopRunning);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
}

FVector ASTUBaseCharacter::GetInputVelocity() const
{
	return InputVelocity;
}

FVector ASTUBaseCharacter::GetRelativeVelocity() const
{
	return GetActorRotation().UnrotateVector(GetVelocity());
}

void ASTUBaseCharacter::OnDeath()
{
	HealthTextComponent->SetText(FText::FromString("Dead"));
	HealthTextComponent->SetTextRenderColor(FColor::Red);

	PlayAnimMontage(DeathAnimMontage);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("BlockAll");
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponComponent->StopFire();

	GetCharacterMovement()->SetMovementMode(MOVE_None);
	SetLifeSpan(DeathLifeSpan);

	Controller->ChangeState(NAME_Spectating);

	UE_LOG(LogBaseCharacter, Error, TEXT("Is Dead"));
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float MaxHealth)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

	if (Health >= MaxHealth / 2)
		HealthTextComponent->SetTextRenderColor(FColor::Green);
	else if (Health >= MaxHealth / 4)
		HealthTextComponent->SetTextRenderColor(FColor::Yellow);
	else
		HealthTextComponent->SetTextRenderColor(FColor::Orange);

	UE_LOG(LogBaseCharacter, Display, TEXT("Health Changed : %.2f"), Health);
}

void ASTUBaseCharacter::OnLand(const FHitResult& Hit)
{
	float FallVelocityZ = -GetVelocity().Z;

	if (FallVelocityZ > FallDamageVelocity.X)
	{
		PlayAnimMontage(FallAnimMontage);

		GetCharacterMovement()->SetMovementMode(MOVE_None);

		FTimerHandle LandedTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LandedTimerHandle, [&LandedTimerHandle, this]
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				GetWorld()->GetTimerManager().ClearTimer(LandedTimerHandle);
			}, FallDelay, false);

		float Damage = FMath::GetMappedRangeValueClamped(FallDamageVelocity, FallDamage, FallVelocityZ);
		TakeDamage(Damage, {}, nullptr, nullptr);
	}

	UE_LOG(LogBaseCharacter, Display, TEXT("Landed with Velocity: %.f"), FallVelocityZ);
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	InputVelocity.X = Amount;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	InputVelocity.Y = Amount;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::LookUp(float Amount)
{
	AddControllerPitchInput(Amount);
}

void ASTUBaseCharacter::LookAround(float Amount)
{
	AddControllerYawInput(Amount);
}
