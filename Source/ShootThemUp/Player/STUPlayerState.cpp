// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPlayerState.h"

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