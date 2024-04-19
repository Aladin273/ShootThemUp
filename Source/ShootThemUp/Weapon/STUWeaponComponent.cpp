// Fill out your copyright notice in the Description page of Project Settings.

#include "STUWeaponComponent.h"

#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentWeapon)
		CurrentWeapon->Destroy();
}

void USTUWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTUWeaponComponent::StartFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StopFire();
}

void USTUWeaponComponent::SpawnWeapon()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);

		if (CurrentWeapon)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocket);
			CurrentWeapon->SetOwner(Character);
		}
	}
}

