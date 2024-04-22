// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseWeapon::StartFire()
{
	MakeShot();
}

void ASTUBaseWeapon::StopFire()
{
	// Possible Override
}

bool ASTUBaseWeapon::IsFiring() const
{
    return false;
}

void ASTUBaseWeapon::Reload()
{
	if (!CurrentAmmo.bInfinite)
	{
		if (CurrentAmmo.Clips == 0)
			return;

		CurrentAmmo.Clips = CurrentAmmo.Clips - 1;
	}

	CurrentAmmo.Bullets = DefaultAmmo.Bullets;	
	UE_LOG(LogBaseWeapon, Display, TEXT("Ammo: Reload"));
	
	StopFire();
	LogAmmo();
}

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && (CurrentAmmo.Clips > 0 || CurrentAmmo.bInfinite);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets > 0)
	{
		CurrentAmmo.Bullets--;
		LogAmmo();

		if (IsClipEmpty() && !IsAmmoEmpty())
		{
			StopFire();
			OnEmptyClip.Broadcast();
		}
	}
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo;

	AmmoInfo += "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

void ASTUBaseWeapon::MakeShot()
{
	FVector TraceStart, TraceEnd;

	if (IsAmmoEmpty() || !GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0.f, 3.f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 3.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Blue, false, 1.0f, 0.f, 3.f);
	}

	DecreaseAmmo();
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        TraceStart = ViewLocation;
        TraceEnd = TraceStart + ViewRotation.Vector() * TraceMaxDistance;

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