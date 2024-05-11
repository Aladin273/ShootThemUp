// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUGameModeBase.generated.h"

class APawn;
class AAIController;
class USoundCue;

UENUM(BlueprintType)
enum class ESTUMatchState : uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	GameOver
};

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "64"))
	int32 PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "12"))
	int32 RoundsNum = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "600"))
	int32 RoundTime = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	bool bAllowRespawn = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "30", EditCondition = "bAllowRespawn"))
	int32 RespawnTime = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0"))
	TArray<FLinearColor> TeamsInfo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESTUMatchState, MatchState);

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTUGameModeBase();

	virtual void StartPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	virtual bool ClearPause() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetTotalRoundsNum() const;

	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetRoundTimeRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void RespawnRequest(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void Killed(AController* Killer, AController* Victim);

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void LogInfo();

public:
	FOnMatchStateChangedSignature OnMatchStateChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	USoundCue* PlayerDeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	USoundCue* PlayerRespawnSound;

private:
	void SpawnBots();
	void CreateTeams();

	void SetPlayerColor(AController* InController);

	void StartRound();
	void UpdateRound();

	void ResetPlayer(AController* InController);
	void ResetPlayers();

	void RespawnPlayer(AController* InController);

	void GameOver();

	void SetMatchState(ESTUMatchState State);

	void StopAllFire();
	
	ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
	
	int32 CurrentRound = 1;

	int32 RoundCountDown = 0;
	FTimerHandle RoundTimerHandle;
};
