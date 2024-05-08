// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUPlayerWidget.generated.h"

struct FWeaponUIData;
struct FWeaponAmmoData;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerWidget : public USTUBaseWidget
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

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnTakeDamage();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnTakeHeal();

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HealAnimation;

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnNewPawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(float Health, float HealthDelta, float MaxHealth);
};
