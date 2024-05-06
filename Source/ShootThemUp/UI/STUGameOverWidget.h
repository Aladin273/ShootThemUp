// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameOverWidget.generated.h"

class UButton;
class USTUStatTableWidget;
enum class ESTUMatchState : uint8;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* B_Restart;

	UPROPERTY(meta = (BindWidget))
	UButton* B_MainMenu;

    UPROPERTY(meta = (BindWidget))
	USTUStatTableWidget* WBP_StatTable;

protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnMatchStateChanged(ESTUMatchState State);	

private:
	UFUNCTION()
	void OnRestart();

	UFUNCTION()
	void OnMainMenu();
};
