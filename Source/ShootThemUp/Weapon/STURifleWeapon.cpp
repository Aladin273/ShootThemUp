// Fill out your copyright notice in the Description page of Project Settings.

#include "STURifleWeapon.h"
#include "Components/STUWeaponFXComponent.h"
#include "GameFramework/PlayerController.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifleWeapon::ASTURifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
	InitMuzzleFX();
	bWantsToFire = true;
}

void ASTURifleWeapon::StopFire()
{
	bWantsToFire = false;
	SetMuzzleFXVisibility(false);
}

bool ASTURifleWeapon::IsFiring() const
{
	return bWantsToFire;
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTURifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastShotElapsed = FMath::Clamp(LastShotElapsed + DeltaTime, 0.f, ShotRate);
	LastRecoilElapsed = FMath::Clamp(LastRecoilElapsed + DeltaTime, 0.f, RecoilDuration);

	if (bWantsToFire && LastShotElapsed >= ShotRate)
	{
		MakeShot();
		bWantsToRecoil = bWantsToFire;
		
		LastShotElapsed = 0.f;
		LastRecoilElapsed = 0.f;

		RecoilTargetVertical = FMath::FRandRange(-RecoilVertical / 2, -RecoilVertical);
		RecoilTargetHorizontal = FMath::FRandRange(-RecoilHorizontal, RecoilHorizontal);
	}

	if (bWantsToRecoil && LastRecoilElapsed < RecoilDuration)
	{
		APlayerController* Controller = GetPlayerController();

		if (Controller)
		{
			Controller->AddPitchInput((RecoilTargetVertical * DeltaTime) / RecoilDuration);
			Controller->AddYawInput((RecoilTargetHorizontal * DeltaTime) / RecoilDuration);
		}
	}
	else
	{
		bWantsToRecoil = false;
		LastRecoilElapsed = 0.f;
	}
}

void ASTURifleWeapon::MakeShot()
{
	FVector TraceStart, TraceEnd;
	
	if (IsClipEmpty() || !GetTraceData(TraceStart, TraceEnd))
	{
		if (!IsAmmoEmpty())
			OnEmptyClip.Broadcast(this);

		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		AActor* DamagedActor = HitResult.GetActor();

		if (DamagedActor)
			DamagedActor->TakeDamage(DamageAmount, {}, GetPlayerController(), this);

		WeaponFXComponent->PlayImpactFX(HitResult);

		//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0.f, 3.f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 3.0f);
		
		SpawnTraceFX(GetMuzzleWorldLocation(), HitResult.ImpactPoint);
	}
	else
	{
		//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Blue, false, 1.0f, 0.f, 3.f);
		
		SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);
	}
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	return Super::GetTraceData(TraceStart, TraceEnd);
}

void ASTURifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible, true);
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);

	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}
