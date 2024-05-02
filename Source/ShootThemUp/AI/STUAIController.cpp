// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIController.h"
#include "STUAICharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "../Components/STUAIPerceptionComponent.h"
#include "../Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("AIPerceptionComponent");
    SetPerceptionComponent(*AIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");

    bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto AICharacter = Cast<ASTUAICharacter>(InPawn);

    if (AICharacter)
        RunBehaviorTree(AICharacter->GetBehaviorTreeAsset());
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}