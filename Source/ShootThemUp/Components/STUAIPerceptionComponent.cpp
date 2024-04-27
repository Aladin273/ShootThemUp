// Fill out your copyright notice in the Description page of Project Settings.

#include "STUAIPerceptionComponent.h"

#include "AIController.h"
#include "STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);

    if (PercieveActors.Num() > 0)
    {
        const auto Controller = Cast<AAIController>(GetOwner());

        if (Controller)
        {
            const auto Pawn = Controller->GetPawn();

            if (Pawn)
            {
                float ClosestDistance = MAX_FLT;
                AActor* ClosestActor = nullptr;

                for (auto PercieveActor : PercieveActors)
                {
                    const auto HealthComponent = PercieveActor->FindComponentByClass<USTUHealthComponent>();

                    if (HealthComponent && !HealthComponent->IsDead())
                    {
                        const auto Distance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Length();

                        if (Distance < ClosestDistance)
                        {
                            ClosestDistance = Distance;
                            ClosestActor = PercieveActor;
                        }
                    }
                }

                return ClosestActor;
            }
        }
    }

    return nullptr;
}
