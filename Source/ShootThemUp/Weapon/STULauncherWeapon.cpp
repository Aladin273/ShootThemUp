// Fill out your copyright notice in the Description page of Project Settings.

#include "STULauncherWeapon.h"

ASTULauncherWeapon::ASTULauncherWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

}
void ASTULauncherWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTULauncherWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTULauncherWeapon::StartFire()
{
	Super::StartFire();
}

void ASTULauncherWeapon::StopFire()
{
	Super::StopFire();
}

void ASTULauncherWeapon::MakeShot()
{
	Super::MakeShot();
}

bool ASTULauncherWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	return Super::GetTraceData(TraceStart, TraceEnd);
}