// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../STUGameInstance.h"

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (B_StartGame)
        B_StartGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);

    if (B_QuitGame)
        B_QuitGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
}

void USTUMenuWidget::OnStartGame()
{
    const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

    if (GameInstance && !GameInstance->GetStartupLevelName().IsNone())
    {
        UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevelName());
    }
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
