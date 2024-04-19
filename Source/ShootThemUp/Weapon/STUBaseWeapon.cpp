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
	// TODO
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

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();

	if (DamagedActor)
		DamagedActor->TakeDamage(DamageAmount, {}, GetPlayerController(), this);
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

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocket);
}