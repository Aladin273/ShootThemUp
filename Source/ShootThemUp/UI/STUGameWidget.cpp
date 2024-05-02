// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameWidget.h"

#include "../STUGameModeBase.h"
#include "../Player/STUPlayerState.h"

int32 USTUGameWidget::GetKillsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USTUGameWidget::GetDeathsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

int32 USTUGameWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 USTUGameWidget::GetTotalRoundsNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetTotalRoundsNum() : 0;
}

int32 USTUGameWidget::GetRoundTimeRemaining() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundTimeRemaining() : 0;
}

ASTUGameModeBase* USTUGameWidget::GetSTUGameMode() const
{
    return Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
}

ASTUPlayerState* USTUGameWidget::GetSTUPlayerState() const
{
    return Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState);
}
