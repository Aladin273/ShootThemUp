// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPlayerWidget.h"

#include "../Components/STUHealthComponent.h"
#include "../Components/STUWeaponComponent.h"

#include "../STUUtils.h"

float USTUPlayerWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.f;
}

FWeaponUIData USTUPlayerWidget::GetWeaponUIData() const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent ? WeaponComponent->GetUIData() : FWeaponUIData{};
}

FWeaponAmmoData USTUPlayerWidget::GetWeaponAmmoData() const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent ? WeaponComponent->GetAmmoData() : FWeaponAmmoData{};
}

bool USTUPlayerWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent ? !HealthComponent->IsDead() : false;
}

bool USTUPlayerWidget::IsPlayerSpectating() const
{
    const AController* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerWidget::Initialize()
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    if (HealthComponent)
        HealthComponent->OnHealthChanged.AddDynamic(this, &USTUPlayerWidget::OnHealthChanged);

    return Super::Initialize();
}

void USTUPlayerWidget::OnHealthChanged(float Health, float HealthDelta, float MaxHealth)
{
    if (HealthDelta < 0.f)
        OnTakeDamage();
    else
        OnTakeHeal();
}
