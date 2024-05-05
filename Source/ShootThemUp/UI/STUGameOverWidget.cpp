// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameOverWidget.h"

#include "Components/VerticalBox.h"
#include "Components/Button.h"

#include "../STUGameModeBase.h"
#include "../Player/STUPlayerState.h"

#include "../UI/STUStatRowWidget.h"

#include "Kismet/GameplayStatics.h"
#include "../STUGameInstance.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

        if (GameMode)
            GameMode->OnMatchStateChanged.AddDynamic(this, &USTUGameOverWidget::OnMatchStateChanged);
    }

    if (B_Restart)
        B_Restart->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnRestart);


    if (B_MainMenu)
        B_MainMenu->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnMainMenu);
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

void USTUGameOverWidget::OnRestart()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void USTUGameOverWidget::OnMainMenu()
{
    const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

    if (GameInstance && !GameInstance->GetMenuLevelName().IsNone())
    {
        UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
    }
}
