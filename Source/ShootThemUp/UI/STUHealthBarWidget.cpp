// Fill out your copyright notice in the Description page of Project Settings.

#include "STUHealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "../Player/STUPlayerState.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
    if (PB_HealthBar)
    {
        PB_HealthBar->SetPercent(Percent);

        if (FMath::IsNearlyZero(Percent))
        {
            PB_HealthBar->SetVisibility(ESlateVisibility::Visible);

            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&TimerHandle, this]
                { 
                    PB_HealthBar->SetVisibility(ESlateVisibility::Hidden); 
                    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
                }, DurationVisibility, false);
        }
        else
        {
            PB_HealthBar->SetVisibility(Percent > PercentVisibility ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
        }
     
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
