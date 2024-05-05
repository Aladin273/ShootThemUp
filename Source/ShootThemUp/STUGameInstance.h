// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FName GetMenuLevelName() const { return MenuLevelName; };

	UFUNCTION(BlueprintCallable)
	FName GetStartupLevelName() const { return StartupLevelName; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FName StartupLevelName = NAME_None;
};
