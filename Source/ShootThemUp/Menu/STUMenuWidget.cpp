// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"

#include "Sound/SoundCue.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "../STUGameInstance.h"

#include "STULevelItemWidget.h"

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (B_StartGame)
        B_StartGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);

    if (B_QuitGame)
        B_QuitGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);

    const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

    if (GameInstance)
    {
        checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));

        if (HB_LevelItems)
        {
            HB_LevelItems->ClearChildren();

            for (auto LevelData : GameInstance->GetLevelsData())
            {
                const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);

                if (LevelItemWidget)
                {
                    LevelItemWidget->SetLevelData(LevelData);
                    LevelItemWidget->OnLevelSelected.AddDynamic(this, &USTUMenuWidget::OnLevelSelected);
                    
                    HB_LevelItems->AddChild(LevelItemWidget);
                    LevelItemWidgets.Add(LevelItemWidget);
                }
            }
        }

        if (GameInstance->GetStartupLevel().LevelName.IsNone())
        {
            OnLevelSelected(GameInstance->GetLevelsData()[0]);
        }
        else
        {
            OnLevelSelected(GameInstance->GetStartupLevel());
        }
    }
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation == LoadingAnimation)
    {
        const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

        if (GameInstance)
        {
            UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevel().LevelName);
        }
    }
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();

    if (GameInstance)
    {
        GameInstance->SetStartupLevel(Data);

        for (auto LevelItemWidget : LevelItemWidgets)
        {
            if (LevelItemWidget)
            {
                const auto bIsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
                LevelItemWidget->SetSelected(bIsSelected);
            }
        }
    }
}

void USTUMenuWidget::OnStartGame()
{
    PlayAnimation(LoadingAnimation);
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
