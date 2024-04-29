// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIWeaponComponent.h"

void USTUAIWeaponComponent::StartFire()
{
    if (CanFire())
    {
        if (CurrentWeapon.Weapon->IsAmmoEmpty())
            NextWeapon();
        else
            CurrentWeapon.Weapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    if (CanEquip())
    {
        int32 NextIndex = CurrentWeaponIndex;

        do 
        {
            NextIndex = (NextIndex + 1) % Weapons.Num();

            if (!Weapons[NextIndex].Weapon->IsAmmoEmpty())
            {
                CurrentWeaponIndex = NextIndex;
                EquipWeapon(CurrentWeaponIndex);

                break;
            }
        } 
        while (NextIndex != CurrentWeaponIndex);
    }
}
