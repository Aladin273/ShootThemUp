// Fill out your copyright notice in the Description page of Project Settings.

#include "STUNextLocationTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Controller && Blackboard)
    {
        const auto Pawn = Controller->GetPawn();

        if (Pawn)
        {
            const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);

            if (NavSystem)
            {
                FNavLocation NavLocation;
                
                if (NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavLocation))
                {
                    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;
}
