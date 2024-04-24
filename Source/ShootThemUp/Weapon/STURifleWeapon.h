// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"

#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	ASTURifleWeapon();

public:
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual bool IsFiring() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	float DamageAmount = 26.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ShotRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RecoilDuration = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	float RecoilVertical = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	float RecoilHorizontal = 0.125f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;

private:
	bool bWantsToFire = false;
	bool bWantsToRecoil = false;
	
	float LastShotElapsed = 0.f;
	float LastRecoilElapsed = 0.f;

	float RecoilTargetVertical = 0.f;
	float RecoilTargetHorizontal = 0.f;
};