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

	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;

	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}

	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
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

void USTUWeaponComponent::NextWeapon()
{
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		for (auto WeaponClass : WeaponClasses)
		{
			ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);

			if (Weapon)
			{
				Weapons.Add(Weapon);
				Weapon->SetOwner(Character);
				AttachToSocket(Weapon, Character->GetMesh(), WeaponArmorySocket);
			}
		}
	}
}

void USTUWeaponComponent::EquipWeapon(int32 Index)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
			AttachToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocket);
		}

		CurrentWeapon = Weapons[Index];
		AttachToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocket);
	}
}

void USTUWeaponComponent::AttachToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Component, AttachmentRules, SocketName);

}

