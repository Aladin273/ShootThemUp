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
    if (I_LevelPreview)
        I_LevelPreview->SetColorAndOpacity(bIsSelected ? FLinearColor::Red : FLinearColor::White);
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (B_SelectLevel)
    {
        B_SelectLevel->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
        B_SelectLevel->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHovered);
        B_SelectLevel->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnHovered);
    }
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::OnLevelItemHovered()
{
    if (I_LevelFrame)
    {
        I_LevelFrame->SetVisibility(ESlateVisibility::Visible);
    }
}

void USTULevelItemWidget::OnLevelItemUnHovered()
{
    if (I_LevelFrame)
    {
        I_LevelFrame->SetVisibility(ESlateVisibility::Hidden);
    }
}
