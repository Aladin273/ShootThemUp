// Fill out your copyright notice in the Description page of Project Settings.


#include "STUNeedAmmoDecorator.h"

#include "AIController.h"
#include "../../Components/STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();

        if (WeaponComponent)
        {
            return WeaponComponent->IsNeedAmmo(WeaponClass);
        }
    }

    return false;
}
