// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

	FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

	if (HitResult.bBlockingHit)
	{	
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 3.0f);
	}

	DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), EndPoint, FColor::Blue, false, 1.0f, 0.f, 3.f);
	
	DecreaseAmmo();
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (GetViewPoint(ViewLocation, ViewRotation))
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

AController* ASTUBaseWeapon::GetController() const
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

bool ASTUBaseWeapon::GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character->IsPlayerControlled())
	{
		APlayerController* Controller = Cast<APlayerController>(GetController());

		if (Controller)
		{
			Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
			return true;
		}

		return false;
	}
	
	ViewLocation = WeaponMesh->GetSocketLocation(MuzzleSocket);
	ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocket);
	
	return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMesh, MuzzleSocket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);

	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}

	return TraceFXComponent;
}