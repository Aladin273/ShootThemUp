// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUUserWidget.generated.h"

struct FWeaponUIData;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	FWeaponUIData GetWeaponUIData() const;

	UFUNCTION(BlueprintCallable)
	FAmmoData GetWeaponAmmoData() const;

	UFUNCTION(BlueprintCallable)
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsPlayerSpectating() const;
};
