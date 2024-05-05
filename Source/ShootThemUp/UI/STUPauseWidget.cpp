// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPauseWidget.h"

#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"
#include "../STUGameInstance.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (B_ClearPause)
        B_ClearPause->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);

    if (B_MainMenu)
        B_MainMenu->OnClicked.AddDynamic(this, &USTUPauseWidget::OnMainMenu);
}

void USTUPauseWidget::OnClearPause()
{
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->ClearPause();
    }
}

void USTUPauseWidget::OnMainMenu()
{
    const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

    if (GameInstance && !GameInstance->GetMenuLevelName().IsNone())
    {
        UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
    }
}
