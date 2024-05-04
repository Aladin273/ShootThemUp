// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameOverWidget.h"

#include "Components/VerticalBox.h"

#include "../STUGameModeBase.h"
#include "../Player/STUPlayerState.h"

#include "../UI/STUStatRowWidget.h"

bool USTUGameOverWidget::Initialize()
{
    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

        if (GameMode)
            GameMode->OnMatchStateChanged.AddDynamic(this, &USTUGameOverWidget::OnMatchStateChanged);
    }

    return Super::Initialize();
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver)
    {
        if (GetWorld() && VB_StatBox)
        {
            VB_StatBox->ClearChildren();

            for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
            {
                const auto Controller = Iterator->Get();

                if (Controller)
                {
                    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);

                    if (PlayerState)
                    {
                        const auto StatRowWidget = CreateWidget<USTUStatRowWidget>(GetWorld(), StatRowWidgetClass);

                        if (StatRowWidget)
                        {
                            StatRowWidget->SetName(FText::FromString(PlayerState->GetPlayerName()));
                            StatRowWidget->SetKills(FText::FromString(FString::FromInt(PlayerState->GetKillsNum())));
                            StatRowWidget->SetDeaths(FText::FromString(FString::FromInt(PlayerState->GetDeathsNum())));
                            StatRowWidget->SetTeam(FText::FromString(FString::FromInt(PlayerState->GetTeamID())));
                            StatRowWidget->SetIndicator(Controller->IsPlayerController());

                            VB_StatBox->AddChild(StatRowWidget);
                        }
                    }
                }
            }
        }
    }
}
