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

	CurrentAmmo = AmmoData;
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
	int32 BulletsToReload = AmmoData.Bullets - CurrentAmmo.Bullets;

	if (!CurrentAmmo.bInfinite)
	{
		if (CurrentAmmo.Stock == 0)
			return;
		else if (CurrentAmmo.Stock < BulletsToReload)
			BulletsToReload = CurrentAmmo.Stock;

		CurrentAmmo.Stock -= BulletsToReload;
	}

	CurrentAmmo.Bullets += BulletsToReload;
	UE_LOG(LogBaseWeapon, Display, TEXT("Ammo: Reload"));
	
	LogAmmo();
}

bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < AmmoData.Bullets && (CurrentAmmo.Stock > 0 || CurrentAmmo.bInfinite);
}

FWeaponUIData ASTUBaseWeapon::GetUIData() const
{
	return UIData;
}

FWeaponAmmoData ASTUBaseWeapon::GetAmmoData() const
{
	return CurrentAmmo;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 Bullets)
{
	if (!CurrentAmmo.bInfinite && CurrentAmmo.Stock < AmmoData.Stock)
	{
		CurrentAmmo.Stock = FMath::Clamp(CurrentAmmo.Stock + Bullets, 0, AmmoData.Stock);

		if (IsClipEmpty())
			OnEmptyClip.Broadcast(this);

		return true;
	}

	return false;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets > 0)
	{
		CurrentAmmo.Bullets--;
		LogAmmo();

        if (IsClipEmpty() && !IsAmmoEmpty())
        {
			OnEmptyClip.Broadcast(this);;
            StopFire();
        }
	}
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Bullets == AmmoData.Bullets && (CurrentAmmo.Stock == AmmoData.Stock || CurrentAmmo.bInfinite);
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.Stock == 0 && CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo;

	AmmoInfo += "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.Stock);

	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

void ASTUBaseWeapon::MakeShot()
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
	CollisionParams.bReturnPhysicalMaterial = true;

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