// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	int32 GetTeamID() const;
	
	UFUNCTION(BlueprintCallable)
	const FLinearColor& GetTeamColor() const;

	UFUNCTION(BlueprintCallable)
	void SetTeamID(int32 ID);
	
	UFUNCTION(BlueprintCallable)
	void SetTeamColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable)
	void AddKill();
	
	UFUNCTION(BlueprintCallable)
	void AddDeath();

	UFUNCTION(BlueprintCallable)
	int32 GetKillsNum() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetDeathsNum() const;

	UFUNCTION(BlueprintCallable)
	void LogInfo() const;

private:
	int32 TeamID;
	FLinearColor TeamColor;

	int32 KillsNum = 0;
	int32 DeathsNum = 0;
};
