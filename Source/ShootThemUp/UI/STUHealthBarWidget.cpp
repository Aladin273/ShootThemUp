// Fill out your copyright notice in the Description page of Project Settings.

#include "STUHealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "../Player/STUPlayerState.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
    if (PB_HealthBar)
    {
        PB_HealthBar->SetPercent(Percent);

        PB_HealthBar->SetVisibility(((Percent > PercentVisibility) || FMath::IsNearlyZero(Percent)) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
     
        if (bUsePercentBasedColor && Percent <= PercentCritical)
        {
            PB_HealthBar->SetFillColorAndOpacity(ColorCritical);
        }
        else if (bUsePercentBasedColor && Percent <= PercentWarning)
        {
            PB_HealthBar->SetFillColorAndOpacity(ColorWarning);
        }
        else
        {
            PB_HealthBar->SetFillColorAndOpacity(ColorDefault);
        }
    }
}
