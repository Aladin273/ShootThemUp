// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameOverWidget.h"

#include "Components/Button.h"

#include "../STUGameInstance.h"
#include "../STUGameModeBase.h"
#include "STUStatTableWidget.h"

#include "Kismet/GameplayStatics.h"

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
        if (WBP_StatTable)
        {
            WBP_StatTable->Refresh();
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
