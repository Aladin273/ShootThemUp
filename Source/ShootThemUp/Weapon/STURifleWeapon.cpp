// Fill out your copyright notice in the Description page of Project Settings.

#include "STURifleWeapon.h"
#include "GameFramework/PlayerController.h"

ASTURifleWeapon::ASTURifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASTURifleWeapon::StartFire()
{
	bWantsToFire = true;
}

void ASTURifleWeapon::StopFire()
{
	bWantsToFire = false;
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

		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0.f, 3.f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 3.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Blue, false, 1.0f, 0.f, 3.f);
	}

	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	return Super::GetTraceData(TraceStart, TraceEnd);
}