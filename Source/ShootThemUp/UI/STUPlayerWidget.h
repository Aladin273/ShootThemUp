// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerWidget.generated.h"

struct FWeaponUIData;
struct FWeaponAmmoData;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	FWeaponUIData GetWeaponUIData() const;

	UFUNCTION(BlueprintCallable)
	FWeaponAmmoData GetWeaponAmmoData() const;

	UFUNCTION(BlueprintCallable)
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTakeDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTakeHeal();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnHealthChanged(float Health, float HealthDelta, float MaxHealth);
};
