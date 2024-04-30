// Fill out your copyright notice in the Description page of Project Settings.


#include "STUHealthPercentDecorator.h"

#include "AIController.h"
#include "../../Components/STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (Controller)
    {
        const auto HealthComponent = Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>();

        if (HealthComponent && !HealthComponent->IsDead())
        {
            return HealthComponent->GetHealthPercent() < HealthPercent;
        }
    }

    return false;
}
