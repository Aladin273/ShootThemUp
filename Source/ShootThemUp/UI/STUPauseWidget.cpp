// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPauseWidget.h"

#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

bool USTUPauseWidget::Initialize()
{
    const auto Init = Super::Initialize();

    if (B_ClearPause)
    {
        B_ClearPause->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }

    return Init;
}

void USTUPauseWidget::OnClearPause()
{
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->ClearPause();
    }
}
