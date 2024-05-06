// Fill out your copyright notice in the Description page of Project Settings.

#include "STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (T_LevelName)
    {
        T_LevelName->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if (I_LevelPreview)
    {
        I_LevelPreview->SetBrushFromTexture(Data.LevelThumbnail);
    }
}

FLevelData USTULevelItemWidget::GetLevelData() const
{
    return LevelData;
}

void USTULevelItemWidget::SetSelected(bool bIsSelected)
{
    if (I_LevelFrame)
    {
        I_LevelFrame->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (B_SelectLevel)
        B_SelectLevel->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}
