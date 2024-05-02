// Fill out your copyright notice in the Description page of Project Settings.


#include "STUSpectatorWidget.h"

#include "../Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTimeRemaining(int32& CountDown)
{
    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();
    
    if (RespawnComponent && RespawnComponent->IsRespawnInProgress())
    {
        CountDown = RespawnComponent->GetRespawnTimeRemaining();
        return true;
    }

    return false;
}
