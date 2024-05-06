// Fill out your copyright notice in the Description page of Project Settings.

#include "STUStatTableWidget.h"

#include "Components/VerticalBox.h"
#include "../UI/STUStatRowWidget.h"

#include "../STUGameModeBase.h"
#include "../Player/STUPlayerState.h"

void USTUStatTableWidget::RefreshStats()
{
    if (GetWorld() && VB_StatBox)
    {
        VB_StatBox->ClearChildren();

        for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
        {
            const auto Controller = Iterator->Get();
            if (!Controller) continue;
            
            const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
            if (!PlayerState) continue;
            
            const auto StatRowWidget = CreateWidget<USTUStatRowWidget>(GetWorld(), StatRowWidgetClass);
            if (!StatRowWidget) continue;
            
            StatRowWidget->SetName(FText::FromString(PlayerState->GetPlayerName()));
            StatRowWidget->SetKills(FText::FromString(FString::FromInt(PlayerState->GetKillsNum())));
            StatRowWidget->SetDeaths(FText::FromString(FString::FromInt(PlayerState->GetDeathsNum())));
            StatRowWidget->SetTeam(FText::FromString(FString::FromInt(PlayerState->GetTeamID())));
            StatRowWidget->SetIndicator(Controller->IsPlayerController());

            VB_StatBox->AddChild(StatRowWidget);
        }
    }
}

void USTUStatTableWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    RefreshStats();
}
