// Fill out your copyright notice in the Description page of Project Settings.


#include "STUUserWidget.h"

#include "../Player/STUHealthComponent.h"
#include "../Weapon/STUWeaponComponent.h"

float USTUUserWidget::GetHealthPercent() const
{
    const APawn* Player = GetOwningPlayerPawn();

    if (Player)
    {
        const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
        const auto HealthComponent = Cast<USTUHealthComponent>(Component);

        if (HealthComponent)
            return HealthComponent->GetHealthPercent();
    }

    return 0.0f;
}

FWeaponUIData USTUUserWidget::GetWeaponUIData() const
{
    const APawn* Player = GetOwningPlayerPawn();

    if (Player)
    {
        const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
        const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);

        if (WeaponComponent)
            return WeaponComponent->GetUIData();
    }

    return {};
}

FAmmoData USTUUserWidget::GetWeaponAmmoData() const
{
    const APawn* Player = GetOwningPlayerPawn();

    if (Player)
    {
        const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
        const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);

        if (WeaponComponent)
            return WeaponComponent->GetAmmoData();
    }

    return {};
}

bool USTUUserWidget::IsPlayerAlive() const
{
    const APawn* Player = GetOwningPlayerPawn();

    if (Player)
    {
        const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
        const auto HealthComponent = Cast<USTUHealthComponent>(Component);

        if (HealthComponent)
            return !HealthComponent->IsDead();
    }

    return false;
}

bool USTUUserWidget::IsPlayerSpectating() const
{
    const AController* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
