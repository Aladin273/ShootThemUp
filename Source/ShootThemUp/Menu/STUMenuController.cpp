// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuController.h"

void ASTUMenuController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
