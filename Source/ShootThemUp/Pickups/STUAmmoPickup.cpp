// Fill out your copyright notice in the Description page of Project Settings.

#include "STUAmmoPickup.h"

#include "../Components/STUHealthComponent.h"
#include "../Components/STUWeaponComponent.h"

#include "../STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    
    if (HealthComponent && !HealthComponent->IsDead())
    {
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);

        if (WeaponComponent)
        {
            bool bIsTaken = WeaponComponent->TryToAddAmmo(WeaponClass, BulletsAmount);

            if (bIsTaken)
                UE_LOG(LogAmmoPickup, Display, TEXT("AmmoPickup was taken"));

            return bIsTaken;
        }
    }

    return false;
}
