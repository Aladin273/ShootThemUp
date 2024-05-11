// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPlayerController.h"


#include "../Components/STURespawnComponent.h"
#include "../STUGameModeBase.h"
#include "../STUGameUserSettings.h"

#include "../UI/STUStatTableWidget.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::BeginPlay()
{
    if (GetWorld())
    {
        GameTableWidget = CreateWidget<USTUStatTableWidget>(GetWorld(), GameTableWidgetCass);

        if (GameTableWidget)
        {
            GameTableWidget->AddToViewport();
            GameTableWidget->SetVisibility(ESlateVisibility::Hidden);
        }

        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

        if (GameMode)
            GameMode->OnMatchStateChanged.AddDynamic(this, &ASTUPlayerController::OnMatchStateChanged);
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
        InputComponent->BindAction("Mute", IE_Pressed, this, &ASTUPlayerController::OnMute);
        InputComponent->BindAction("GameTable", IE_Pressed, this, &ASTUPlayerController::OnGameTable);
        InputComponent->BindAction("GameTable", IE_Released, this, &ASTUPlayerController::OnGameTable);
    }
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void ASTUPlayerController::OnPauseGame()
{
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->SetPause(this);
    }
}

void ASTUPlayerController::OnMute()
{
    USTUGameUserSettings::GetSTUGameUserSettings()->SetMuted(!USTUGameUserSettings::GetSTUGameUserSettings()->GetMuted());
    USTUGameUserSettings::GetSTUGameUserSettings()->ApplySettings(true);
}

void ASTUPlayerController::OnGameTable()
{
    if (GameTableWidget)
    {
        if (GameTableWidget->GetVisibility() == ESlateVisibility::Hidden)
        {
            GameTableWidget->RefreshStats();
            GameTableWidget->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            GameTableWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
