// Fill out your copyright notice in the Description page of Project Settings.

#include "STUStatRowWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUStatRowWidget::SetName(const FText& Text)
{
    if (!T_Name) return;
    T_Name->SetText(Text);
}

void USTUStatRowWidget::SetKills(const FText& Text)
{
    if (!T_Kills) return;
    T_Kills->SetText(Text);
}

void USTUStatRowWidget::SetDeaths(const FText& Text)
{
    if (!T_Deaths) return;
    T_Deaths->SetText(Text);
}

void USTUStatRowWidget::SetTeam(const FText& Text)
{
    if (!T_Team) return;
    T_Team->SetText(Text);
}

void USTUStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
    if (!I_TeamColor) return;
    I_TeamColor->SetColorAndOpacity(Color);
}

void USTUStatRowWidget::SetIndicator(bool bVisible)
{
    if (!I_Indicator) return;
    I_Indicator->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
