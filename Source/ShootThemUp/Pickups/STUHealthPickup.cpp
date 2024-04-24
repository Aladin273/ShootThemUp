// Fill out your copyright notice in the Description page of Project Settings.

#include "STUHealthPickup.h"

#include "../Components/STUHealthComponent.h"

#include "../STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    if (HealthComponent && !HealthComponent->IsDead())
    {
        bool bIsTaken = HealthComponent->TryToAddHealth(HealthAmount);

        if (bIsTaken)
            UE_LOG(LogHealthPickup, Display, TEXT("HealthPickup was taken"));

        return bIsTaken;
    }

    return false;
}

