// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUHealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float DurationVisibility = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentVisibility = 0.75f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor ColorDefault = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    bool bUsePercentBasedColor = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (EditCondition = "bUsePercentBasedColor"))
    float PercentWarning = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (EditCondition = "bUsePercentBasedColor"))
    float PercentCritical = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (EditCondition = "bUsePercentBasedColor"))
    FLinearColor ColorWarning = FLinearColor::Yellow;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (EditCondition = "bUsePercentBasedColor"))
    FLinearColor ColorCritical = FLinearColor::Red;

public:
	UFUNCTION(BlueprintCallable)
	void SetHealthPercent(float Percent);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_HealthBar;
};
