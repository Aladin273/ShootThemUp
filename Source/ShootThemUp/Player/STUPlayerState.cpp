// Fill out your copyright notice in the Description page of Project Settings.

#include "STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All)

int32 ASTUPlayerState::GetTeamID() const 
{
    return TeamID;
}

const FLinearColor& ASTUPlayerState::GetTeamColor() const
{
    return TeamColor;
}

void ASTUPlayerState::SetTeamID(int32 ID)
{
    TeamID = ID;
}

void ASTUPlayerState::SetTeamColor(const FLinearColor& Color)
{
    TeamColor = Color;
}

void ASTUPlayerState::AddKill()
{
    ++KillsNum;
}

void ASTUPlayerState::AddDeath()
{
    ++DeathsNum;
}

int32 ASTUPlayerState::GetKillsNum() const
{
    return KillsNum;
}

int32 ASTUPlayerState::GetDeathsNum() const
{
    return DeathsNum;
}

void ASTUPlayerState::LogInfo() const
{
    UE_LOG(LogPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
