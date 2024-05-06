// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUStatTableWidget.generated.h"

class UVerticalBox;

UCLASS()
class SHOOTTHEMUP_API USTUStatTableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Refresh();

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_StatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatRowWidgetClass;

protected:
	virtual void NativeOnInitialized() override;
};
