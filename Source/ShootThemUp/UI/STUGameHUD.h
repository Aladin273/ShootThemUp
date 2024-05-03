// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

enum class ESTUMatchState : uint8;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void OnMatchStateChanged(ESTUMatchState State);

private:
	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;

	UPROPERTY()
	TMap<ESTUMatchState, UUserWidget*> Widgets;
};
