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
	int32 GetTeamID() const;
	const FLinearColor& GetTeamColor() const;

	void SetTeamID(int32 ID);
	void SetTeamColor(const FLinearColor& Color);

private:
	int32 TeamID;
	FLinearColor TeamColor;
};
