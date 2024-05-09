// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static USTUGameUserSettings* GetSTUGameUserSettings()
	{
		return Cast<USTUGameUserSettings>(UGameUserSettings::GetGameUserSettings());
	}

	void ApplySettings(bool bCheckForCommandLineOverrides) override
	{
		Super::ApplySettings(bCheckForCommandLineOverrides);
		
		if (MusicSoundClass && SFXSoundClass)
		{
			MusicSoundClass->Properties.Volume = FMath::Clamp(MusicVolume, 0.f, 1.f);
			SFXSoundClass->Properties.Volume = FMath::Clamp(SFXVolume, 0.f, 1.f);
		}
	}

public:
	UFUNCTION(BlueprintCallable)
	void SetInitLaunch(bool InLaunch) { bInitLaunch = InLaunch; };

	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float InVolume) { MusicVolume = InVolume; };

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float InVolume) { SFXVolume = InVolume; };
	
	
	UFUNCTION(BlueprintCallable)
	bool GetInitLaunch() const { return bInitLaunch; };

	UFUNCTION(BlueprintCallable)
	float GetMusicVolume() const { return MusicVolume; };
	
	UFUNCTION(BlueprintCallable)
	float GetSFXVolume() const { return SFXVolume; };

	
	UFUNCTION(BlueprintCallable)
	void SetMusicSoundClass(USoundClass* InSoundClass) { MusicSoundClass = InSoundClass; }

	UFUNCTION(BlueprintCallable)
	void SetSFXSoundClass(USoundClass* InSoundClass) { SFXSoundClass = InSoundClass; }

	
	UFUNCTION(BlueprintCallable)
	USoundClass* GetMusicSoundClass() const { return MusicSoundClass; }

	UFUNCTION(BlueprintCallable)
	USoundClass* GetSFXSoundClass() const { return SFXSoundClass; }


	UFUNCTION(BlueprintCallable)
	void SetMuted(bool InMuted) 
	{ 
		bMuted = InMuted; 

		if (bMuted)
		{
			LastMusicVolume = MusicVolume;
			LastSFXVolume = SFXVolume;
			
			MusicVolume = 0.f;
			SFXVolume = 0.f;
		}
		else
		{
			MusicVolume = LastMusicVolume;
			SFXVolume = LastSFXVolume;
		}
	};

	UFUNCTION(BlueprintCallable)
	bool GetMuted() const { return bMuted; };

protected:
	UPROPERTY(Config)
	bool bInitLaunch = true;

	UPROPERTY(Config)
	float MusicVolume = 1.0f;

	UPROPERTY(Config)
	float SFXVolume = 1.0f;

private:
	UPROPERTY()
	USoundClass* MusicSoundClass;

	UPROPERTY()
	USoundClass* SFXSoundClass;

	float LastMusicVolume = 1.f;
	float LastSFXVolume = 1.f;
	
	bool bMuted = false;
};