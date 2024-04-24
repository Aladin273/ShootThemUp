// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBaseWeapon;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup") 
	TSubclassOf<ASTUBaseWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup") 
	int32 BulletsAmount;

protected:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
