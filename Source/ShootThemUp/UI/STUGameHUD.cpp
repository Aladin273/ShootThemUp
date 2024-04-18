// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameHUD.h"

#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
    FVector2D Center = FVector2D(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    float LineSize = 1.f;
    float LineThickness = 4.0f;
    FLinearColor LineColor = FLinearColor::White;

    DrawLine(Center.X - LineSize, Center.Y, Center.X + LineSize, Center.Y, LineColor, LineThickness);
    DrawLine(Center.X, Center.Y - LineSize, Center.X, Center.Y + LineSize, LineColor, LineThickness);
}
