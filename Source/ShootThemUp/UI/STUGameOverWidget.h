// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
enum class ESTUMatchState : uint8;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_StatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatRowWidgetClass;

private:
	UFUNCTION()
	void OnMatchStateChanged(ESTUMatchState State);
};
