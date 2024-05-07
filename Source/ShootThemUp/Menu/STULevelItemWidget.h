// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../STUGameInstance.h"
#include "STULevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&, LevelData);

UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetLevelData(const FLevelData& Data);
	
	UFUNCTION(BlueprintCallable)
	FLevelData GetLevelData() const;
	
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bIsSelected);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnLevelSelectedSignature OnLevelSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* B_SelectLevel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_LevelName;

	UPROPERTY(meta = (BindWidget))
	UImage* I_LevelPreview;

	UPROPERTY(meta = (BindWidget))
	UImage* I_LevelFrame;

protected:
	virtual void NativeOnInitialized();

private:
	UFUNCTION()
	void OnLevelItemClicked();

	UFUNCTION()
	void OnLevelItemHovered();

	UFUNCTION()
	void OnLevelItemUnHovered();

	FLevelData LevelData;
};
