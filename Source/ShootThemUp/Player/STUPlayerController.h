// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;
class USTUStatTableWidget;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTUPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTURespawnComponent* RespawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameTableWidgetCass;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
	virtual void OnMatchStateChanged(ESTUMatchState State);

private:
	UFUNCTION()
	void OnPauseGame();

	UFUNCTION()
	void OnMute();
	
	UFUNCTION()
	void OnGameTable();

	UPROPERTY()
	USTUStatTableWidget* GameTableWidget;
};
