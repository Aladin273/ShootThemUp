// Fill out your copyright notice in the Description page of Project Settings.


#include "STUUserWidget.h"

#include "../Components/STUHealthComponent.h"
#include "../Components/STUWeaponComponent.h"

template <typename T>
static T* GetSTUPlayerComponent(APawn* PlayerPawn)
{
    return PlayerPawn ? Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass())) : nullptr;
}

float USTUUserWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.f;
}

FWeaponUIData USTUUserWidget::GetWeaponUIData() const
{
    const auto WeaponComponent = GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent ? WeaponComponent->GetUIData() : FWeaponUIData{};
}

FWeaponAmmoData USTUUserWidget::GetWeaponAmmoData() const
{
    const auto WeaponComponent = GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent ? WeaponComponent->GetAmmoData() : FWeaponAmmoData{};
}

bool USTUUserWidget::IsPlayerAlive() const
{
    const auto HealthComponent = GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent ? !HealthComponent->IsDead() : false;
}

bool USTUUserWidget::IsPlayerSpectating() const
{
    const AController* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
