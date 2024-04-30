// Fill out your copyright notice in the Description page of Project Settings.

#include "STUFocusService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USTUFocusService::USTUFocusService()
{
    NodeName = "Focus";
}

void USTUFocusService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Controller && Blackboard)
    {
        const auto EnemyObject = Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

        if (EnemyObject)
        {
            Controller->SetFocus(Cast<AActor>(EnemyObject));
        }
        else
        {
            Controller->ClearFocus(EAIFocusPriority::Gameplay);
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
