// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUPauseWidget.generated.h"


class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* B_ClearPause;

	UPROPERTY(meta = (BindWidget))
	UButton* B_MainMenu;

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();

	UFUNCTION()
	void OnMainMenu();
};
