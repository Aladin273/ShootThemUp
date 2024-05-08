// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuHUD.h"
#include "../UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass);

    if (MenuWidget)
    {
        MenuWidget->AddToViewport();
        MenuWidget->Show();
    }
}
