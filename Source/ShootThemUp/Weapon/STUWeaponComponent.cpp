// Fill out your copyright notice in the Description page of Project Settings.

#include "STUWeaponComponent.h"

#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

#include "../Animations/STUEquipFinishedAnimNotify.h"

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

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !bEquipAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimInProgress;
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

		PlayAnimMontage(EquipAnimMontage);
		bEquipAnimInProgress = true;
	}
}

void USTUWeaponComponent::AttachToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Component, AttachmentRules, SocketName);

}

void USTUWeaponComponent::InitAnimations()
{
	if (EquipAnimMontage)
	{
		const auto NofityEvents = EquipAnimMontage->Notifies;

		for (auto NofityEvent : NofityEvents)
		{
			auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NofityEvent.Notify);

			if (EquipFinishedNotify)
			{
				EquipFinishedNotify->OnNotified.AddDynamic(this, &USTUWeaponComponent::OnEquipFinished);
			}
		}
	}
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

