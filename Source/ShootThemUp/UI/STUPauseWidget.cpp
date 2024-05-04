// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPauseWidget.h"

#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (B_ClearPause)
    {
        B_ClearPause->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
}

void USTUPauseWidget::OnClearPause()
{
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->ClearPause();
    }
}
