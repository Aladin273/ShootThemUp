#pragma once

#include "Player/STUPlayerState.h"

namespace STUUtils
{
    template <typename T>
    static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        return PlayerPawn ? Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass())) : nullptr;
    }

    bool static AreEnemies(AController* Controller1, AController* Controller2)
    {
        if (Controller1 && Controller2)
        {
            const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
            const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

            return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
        }

        return false;
    }
}