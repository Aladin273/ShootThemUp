// Fill out your copyright notice in the Description page of Project Settings.


#include "STUChangeWeaponService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "../../Components/STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();

        if (WeaponComponent && Probability > 0.f && FMath::FRand() <= Probability)
        {
            WeaponComponent->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
