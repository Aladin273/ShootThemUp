// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTUBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastShotElapsed = FMath::Clamp(LastShotElapsed + DeltaTime, 0.f, ShotRate);
	
	if (bWantToFire && LastShotElapsed >= ShotRate)
	{
		MakeShot();
		LastShotElapsed = 0.f;
	}

	if (LastShotElapsed < ShotRate)
	{
		MakeRecoil(DeltaTime);
	}
}

void ASTUBaseWeapon::StartFire()
{
	bWantToFire = true;
}

void ASTUBaseWeapon::StopFire()
{
	bWantToFire = false;
}

void ASTUBaseWeapon::MakeShot()
{
	FVector TraceStart, TraceEnd;

	if (GetTraceData(TraceStart, TraceEnd))
	{
		FHitResult HitResult;
		MakeHit(HitResult, TraceStart, TraceEnd);
		
		if (HitResult.bBlockingHit)
		{
			MakeDamage(HitResult);

			DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0.f, 3.f);
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 3.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Blue, false, 1.0f, 0.f, 3.f);
		}
	}
}

void ASTUBaseWeapon::MakeRecoil(float DeltaTime)
{
	APlayerController* Controller = GetPlayerController();

	if (Controller)
	{
		Controller->AddPitchInput(FMath::FRandRange(-RecoilVertical, -RecoilVertical / 2) * DeltaTime);
		Controller->AddYawInput(FMath::FRandRange(-RecoilHorizontal, RecoilHorizontal) * DeltaTime);
	}
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocket);
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	
	if (Player)
		return Cast<APlayerController>(Player->GetController());

	return nullptr;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	APlayerController* Controller = GetPlayerController();

	if (Controller)
	{
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
		return true;
	}

	return false;
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	
	if (GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		TraceStart = ViewLocation;
		TraceEnd = TraceStart + ViewRotation.Vector() * TraceMaxDistance;
		
		// ShotSpread
		//TraceEnd = TraceStart + FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(ShotSpread)) * TraceMaxDistance;
		
		return true;
	}

	return false;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();

	if (DamagedActor)
		DamagedActor->TakeDamage(DamageAmount, {}, GetPlayerController(), this);
}

