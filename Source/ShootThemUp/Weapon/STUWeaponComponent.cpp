// Fill out your copyright notice in the Description page of Project Settings.

#include "STUWeaponComponent.h"

#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

#include "../Animations/STUEquipFinishedAnimNotify.h"
#include "../Animations/STUReloadFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimations();

	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;

	for (auto Weapon : Weapons)
	{
		Weapon.Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon.Weapon->Destroy();
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
	if (CanFire())
		CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (CurrentWeapon)
		CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
	if (CanEquip())
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
		EquipWeapon(CurrentWeaponIndex);
	}
}

void USTUWeaponComponent::Reload()
{
	if (CanReload())
	{
		PlayAnimMontage(CurrentReloadAnimMontage);
		bReloadAnimInProgress = true;
		
		CurrentWeapon->Reload();
		CurrentWeapon->StopFire();
	}
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return !bEquipAnimInProgress && !bReloadAnimInProgress && CurrentWeapon && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::InitAnimations()
{
	if (EquipAnimMontage)
	{
		USTUEquipFinishedAnimNotify* EquipFinishedNotify = FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);

		if (EquipFinishedNotify)
			EquipFinishedNotify->OnNotified.AddDynamic(this, &USTUWeaponComponent::OnEquipFinished);
	}

	for (auto WeaponDataIterator : WeaponData)
	{
		if (WeaponDataIterator.ReloadAnimMontage)
		{
			USTUReloadFinishedAnimNotify* ReloadFinishedNotify = FindNotifyByClass<USTUReloadFinishedAnimNotify>(WeaponDataIterator.ReloadAnimMontage);

			if (ReloadFinishedNotify)
				ReloadFinishedNotify->OnNotified.AddDynamic(this, &USTUWeaponComponent::OnReloadFinished);
		}
	}
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		for (auto WeaponDataIterator : WeaponData)
		{
			ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponDataIterator.WeaponClass);

			if (Weapon)
			{
				Weapons.Add({ Weapon, WeaponDataIterator.ReloadAnimMontage });
				Weapon->SetOwner(Character);
				Weapon->OnEmptyClip.AddDynamic(this, &USTUWeaponComponent::Reload);
				AttachToSocket(Weapon, Character->GetMesh(), WeaponArmorySocket);
			}
		}
	}
}

void USTUWeaponComponent::EquipWeapon(int32 Index)
{
	if (Index < 0 || Index >= Weapons.Num())
		return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
			AttachToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocket);
		}

		CurrentWeapon = Weapons[Index].Weapon;
		CurrentReloadAnimMontage = Weapons[Index].ReloadAnimMontage;
		AttachToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocket);

		PlayAnimMontage(EquipAnimMontage);
		bEquipAnimInProgress = true;
	}
}

void USTUWeaponComponent::AttachToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Component, AttachmentRules, SocketName);

}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && AnimMontage)
		Character->PlayAnimMontage(AnimMontage);
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && Character->GetMesh() == MeshComp)
	{
		bEquipAnimInProgress = false;
	}
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && Character->GetMesh() == MeshComp)
	{
		bReloadAnimInProgress = false;
	}
}