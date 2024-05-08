// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseCharacter.h"

#include "Components/CapsuleComponent.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "../Components/STUCharacterMovementComponent.h"
#include "../Components/STUHealthComponent.h"
#include "../Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

	CharacterMovementComponent = Cast<USTUCharacterMovementComponent>(GetCharacterMovement());
}

FLinearColor ASTUBaseCharacter::GetPlayerColor() const
{
	return PlayerColor;
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	PlayerColor = Color;

	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

	if (MaterialInst)
	{
		MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
	}
}

FVector ASTUBaseCharacter::GetMovementInput() const
{
	return MovementInput;
}

FRotator ASTUBaseCharacter::GetRotationInput() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	return PlayerController ? PlayerController->RotationInput : FRotator::ZeroRotator;
}

FVector ASTUBaseCharacter::GetRelativeVelocity() const
{
	return GetActorRotation().UnrotateVector(GetVelocity());
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged(HealthComponent->GetHealth(), 0.f, HealthComponent->GetMaxHealth());

	HealthComponent->OnDeath.AddDynamic(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTUBaseCharacter::OnHealthChanged);
	
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnLand);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("BlockAll");
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponComponent->StopFire();

	GetCharacterMovement()->SetMovementMode(MOVE_None);
	SetLifeSpan(DeathLifeSpan);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());

	UE_LOG(LogBaseCharacter, Error, TEXT("Is Dead"));
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta, float MaxHealth)
{
	UE_LOG(LogBaseCharacter, Display, TEXT("Health Changed : %.2f / %.2f"), Health, MaxHealth);
}

void ASTUBaseCharacter::OnLand(const FHitResult& Hit)
{
	float FallVelocityZ = -GetVelocity().Z;

	if (FallVelocityZ > FallDamageVelocity.X)
	{
		PlayAnimMontage(FallAnimMontage);

		WeaponComponent->StopFire();

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

void ASTUBaseCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// Possible Override
}

