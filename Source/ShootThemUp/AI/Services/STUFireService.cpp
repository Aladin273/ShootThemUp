// Fill out your copyright notice in the Description page of Project Settings.


#include "STUFireService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "../../Components/STUWeaponComponent.h"
#include "../../Components/STUHealthComponent.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
        const auto HealthComponent = Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>();

        if (WeaponComponent && HealthComponent && !HealthComponent->IsDead())
        {
            const bool HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

            if (HasAim)
                WeaponComponent->StartFire();
            else
                WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
