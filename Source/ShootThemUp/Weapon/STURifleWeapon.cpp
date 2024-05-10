// Fill out your copyright notice in the Description page of Project Settings.

#include "STURifleWeapon.h"
#include "Components/STUWeaponFXComponent.h"
#include "GameFramework/PlayerController.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ASTURifleWeapon::ASTURifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
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
		APlayerController* Controller = Cast<APlayerController>(GetController());

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
		UGameplayStatics::SpawnSoundAttached(NoAmmoSound, WeaponMesh, MuzzleSocket);
		
		if (!IsAmmoEmpty())
			OnEmptyClip.Broadcast(this);

		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);

		WeaponFXComponent->PlayImpactFX(HitResult);
		
		//DrawDebugSphere(GetWorld(), EndPoint, 10.f, 24, FColor::Red, false, 3.0f);
	}

	//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), EndPoint, FColor::Blue, false, 1.0f, 0.f, 3.f);
	
	SpawnMuzzleFX();
	SpawnTraceFX(GetMuzzleWorldLocation(), EndPoint);

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocket);

	DecreaseAmmo();
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();

	if (DamagedActor)
	{
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.HitInfo = HitResult;
		DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
	}
}