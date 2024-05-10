// Fill out your copyright notice in the Description page of Project Settings.

#include "STUAIPerceptionComponent.h"

#include "AIController.h"
#include "STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "../STUUtils.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;

    if (PercieveActors.Num() < 0 || !IsEnemyExist(PercieveActors))
    {
        PercieveActors.Empty();
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
    }

    if (PercieveActors.Num() < 0 || !IsEnemyExist(PercieveActors))
    {
        PercieveActors.Empty();
        GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    }

    if (PercieveActors.Num() < 0 || !IsEnemyExist(PercieveActors))
    {
        PercieveActors.Empty();
        GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PercieveActors);
    }

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

                    const auto PercievePawn = Cast<APawn>(PercieveActor);
                    const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);

                    if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
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

bool USTUAIPerceptionComponent::IsEnemyExist(const TArray<AActor*>& PercieveActors) const
{
    for (auto PercieveActor : PercieveActors)
    {
        const auto PercievePawn = Cast<APawn>(PercieveActor);
        
        if (PercievePawn && STUUtils::AreEnemies(Cast<AController>(GetOwner()), PercievePawn->Controller))
            return true;
    }

    return false;
}
