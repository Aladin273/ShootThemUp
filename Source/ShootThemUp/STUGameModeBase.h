// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUGameModeBase.generated.h"

class APawn;
class AAIController;

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "64"))
	int32 PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "12"))
	int32 RoundsNum = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "120"))
	int32 RoundsTime = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0"))
	TArray<FLinearColor> TeamsInfo;
};

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTUGameModeBase();

	virtual void StartPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameData GameData;

private:
	void SpawnBots();

	void StartRound();
	void UpdateRound();

	void ResetPlayer(AController* InController);
	void ResetPlayers();

	void CreateTeams();
	void SetPlayerColor(AController* InController);

	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle RoundTimerHandle;
};
