// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTULevelItemWidget;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* B_StartGame;

	UPROPERTY(meta = (BindWidget))
	UButton* B_QuitGame;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_LevelItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	TArray<USTULevelItemWidget*> LevelItemWidgets;

	UFUNCTION()
	void OnLevelSelected(const FLevelData& Data);

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();
};
