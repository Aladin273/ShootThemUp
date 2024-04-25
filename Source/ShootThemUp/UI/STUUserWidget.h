// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUUserWidget.generated.h"

struct FWeaponUIData;
struct FWeaponAmmoData;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FWeaponUIData GetWeaponUIData() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FWeaponAmmoData GetWeaponAmmoData() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeHeal();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnHealthChanged(float Health, float HealthDelta, float MaxHealth);
};
