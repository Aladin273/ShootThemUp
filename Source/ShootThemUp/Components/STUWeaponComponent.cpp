// Fill out your copyright notice in the Description page of Project Settings.

#include "STUWeaponComponent.h"

#include "GameFramework/Character.h"

#include "../Weapon/STUBaseWeapon.h"

#include "../Animations/STUEquipFinishedAnimNotify.h"
#include "../Animations/STUReloadFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
	if (CanFire())
		CurrentWeapon.Weapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (CurrentWeapon.Weapon)
		CurrentWeapon.Weapon->StopFire();
}

bool USTUWeaponComponent::IsFiring() const
{
	return CurrentWeapon.Weapon && CurrentWeapon.Weapon->IsFiring();
}

void USTUWeaponComponent::NextWeapon(int32 WeaponIndex /*= -1*/)
{
	if (CanEquip() && CurrentWeaponIndex != WeaponIndex)
	{
		CurrentWeaponIndex = (WeaponIndex == -1 ? CurrentWeaponIndex + 1 : WeaponIndex) % Weapons.Num();
		EquipWeapon(CurrentWeaponIndex);
	}
}

void USTUWeaponComponent::Reload()
{
	if (CanReload())
	{
		PlayAnimMontage(CurrentWeapon.ReloadAnimMontage);
		bReloadAnimInProgress = true;
		
		if (CurrentWeapon.Weapon)
			CurrentWeapon.Weapon->StopFire();
	}
}

FWeaponUIData USTUWeaponComponent::GetUIData() const
{
	return CurrentWeapon.Weapon ? CurrentWeapon.Weapon->GetUIData() : FWeaponUIData{};
}

FWeaponAmmoData USTUWeaponComponent::GetAmmoData() const
{
	return CurrentWeapon.Weapon ? CurrentWeapon.Weapon->GetAmmoData() : FWeaponAmmoData{};
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponClass, int32 Bullets)
{
	for (FWeaponDataInternal& Weapon : Weapons)
	{
		if (Weapon.Weapon->IsA(WeaponClass))
		{
			return Weapon.Weapon->TryToAddAmmo(Bullets);
		}
	}

	return false;
}

bool USTUWeaponComponent::IsNeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponClass)
{
	for (FWeaponDataInternal& Weapon : Weapons)
	{
		if (Weapon.Weapon->IsA(WeaponClass))
		{
			return Weapon.Weapon->IsAmmoEmpty();
		}
	}

	return false;
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
	CurrentWeapon.Weapon = nullptr;
	CurrentWeapon.ReloadAnimMontage = nullptr;

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

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon.Weapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return !bEquipAnimInProgress && !bReloadAnimInProgress && CurrentWeapon.Weapon && CurrentWeapon.Weapon->CanReload();
}

void USTUWeaponComponent::EquipWeapon(int32 Index)
{
	if (Index < 0 || Index >= Weapons.Num())
		return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
	{
		if (CurrentWeapon.Weapon)
		{
			CurrentWeapon.Weapon->StopFire();
			AttachToSocket(CurrentWeapon.Weapon, Character->GetMesh(), WeaponArmorySocket);
		}

		PlayAnimMontage(EquipAnimMontage);
		bEquipAnimInProgress = true;
			
		CurrentWeapon = Weapons[Index];
		AttachToSocket(CurrentWeapon.Weapon, Character->GetMesh(), WeaponEquipSocket);
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
				if (bAutoReload)
					Weapon->OnEmptyClip.AddDynamic(this, &USTUWeaponComponent::OnReload);
				AttachToSocket(Weapon, Character->GetMesh(), WeaponArmorySocket);
			}
		}
	}
}

void USTUWeaponComponent::AttachToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Component, AttachmentRules, SocketName);
}

void USTUWeaponComponent::InitAnimations()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character)
		Character->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &USTUWeaponComponent::OnMontageEnded);

	// https://forums.unrealengine.com/t/play-montage-in-c-with-onblendout-oninterrupted-etc/447184/7
	//UPlayMontageCallbackProxy* PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage();
	//PlayMontageCallbackProxy->OnCompleted.AddDynamic();

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
		CurrentWeapon.Weapon->Reload();
	}
}

void USTUWeaponComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		if (Character)
		{
			bEquipAnimInProgress = false;
			bReloadAnimInProgress = false;
		}
	}
}

void USTUWeaponComponent::OnReload(ASTUBaseWeapon* Weapon)
{
	if (Weapon == CurrentWeapon.Weapon)
		Reload();
}
