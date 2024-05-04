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
	UFUNCTION(BlueprintCallable)
	void SetName(const FText& Text);
	
	UFUNCTION(BlueprintCallable)
	void SetKills(const FText& Text);
	
	UFUNCTION(BlueprintCallable)
	void SetDeaths(const FText& Text);
	
	UFUNCTION(BlueprintCallable)
	void SetTeam(const FText& Text);
	
	UFUNCTION(BlueprintCallable)
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
