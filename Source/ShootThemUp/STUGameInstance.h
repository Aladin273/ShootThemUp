// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FName LevelDisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	UTexture2D* LevelThumbnail;
};

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FLevelData> GetLevelsData() const { return LevelsData; };

	UFUNCTION(BlueprintCallable)
	FLevelData GetMenuLevel() const { return MenuLevel; };

	UFUNCTION(BlueprintCallable)
	FLevelData GetStartupLevel() const { return StartupLevel; };

	UFUNCTION(BlueprintCallable)
	void SetStartupLevel(FLevelData LevelData) { StartupLevel = LevelData; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FLevelData MenuLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundClass* SFXSoundClass;

protected:
	virtual void OnStart() override;

private:
	FLevelData StartupLevel;
};
