// Fill out your copyright notice in the Description page of Project Settings.

#include "STUMenuModeBase.h"

#include "STUMenuController.h"
#include "STUMenuHUD.h"

ASTUMenuModeBase::ASTUMenuModeBase()
{
    PlayerControllerClass = ASTUMenuController::StaticClass();
    HUDClass = ASTUMenuHUD::StaticClass();
}
