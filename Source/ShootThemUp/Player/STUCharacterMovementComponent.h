// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable)
	virtual void StartRunning();
	
	UFUNCTION(BlueprintCallable)
	virtual void StopRunning();

	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s", DisplayAfter="MaxWalkSpeedCrouched"))
	float MaxWalkSpeedRunning = 1200.f;

private:
	bool WantsToRun = false;
};
