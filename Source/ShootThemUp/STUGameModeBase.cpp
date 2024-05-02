// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameModeBase.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameModeBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    StartRound();
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::SpawnBots()
{
    for (int32 Index = 0; Index < GameData.PlayersNum - 1; ++Index)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnParams);
        RestartPlayer(AIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundsTime;

    GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ASTUGameModeBase::UpdateRound, 1.0f, true);

    UE_LOG(LogGameModeBase, Display, TEXT("--------- Round %i / %i ---------"), CurrentRound, GameData.RoundsNum);
}

void ASTUGameModeBase::UpdateRound()
{
    if (--RoundCountDown <= 0)
    {
        GetWorldTimerManager().ClearTimer(RoundTimerHandle);

        if (++CurrentRound <= GameData.RoundsNum)
        {
            StartRound();
            ResetPlayers();
        }
        else
        {
            UE_LOG(LogGameModeBase, Display, TEXT("--------- GameOver ---------"));
        }
    }
}

void ASTUGameModeBase::ResetPlayer(AController* InController)
{
    if (InController && InController->GetPawn())
    {
        InController->GetPawn()->Reset();
    }

    RestartPlayer(InController);
}

void ASTUGameModeBase::ResetPlayers()
{
    for (auto Iterator = GetWorld()->GetControllerIterator(); Iterator; ++Iterator)
    {
        ResetPlayer(Iterator->Get());
    }
}
