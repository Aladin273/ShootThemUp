// Fill out your copyright notice in the Description page of Project Settings.


#include "STUSpectatorWidget.h"

#include "../Components/STURespawnComponent.h"
#include "../STUGameModeBase.h"

bool USTUSpectatorWidget::GetRespawnTimeRemaining(int32& CountDown)
{
    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();
    
    if (RespawnComponent && RespawnComponent->IsRespawnInProgress())
    {
        CountDown = RespawnComponent->GetRespawnTimeRemaining();
        return true;
    }
    else
    {
        const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();

        if (GameMode)
        {
            CountDown = GameMode->GetRoundTimeRemaining();
            return true;
        }
    }

    return false;
}
