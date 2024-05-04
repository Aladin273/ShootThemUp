// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTTHEMUP_API USTUStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetName(const FText& Text);
	void SetKills(const FText& Text);
	void SetDeaths(const FText& Text);
	void SetTeam(const FText& Text);
	void SetIndicator(bool bVisible);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Kills;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Deaths;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Team;

	UPROPERTY(meta = (BindWidget))
	UImage* I_Indicator;
};
