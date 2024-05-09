// Fill out your copyright notice in the Description page of Project Settings.


#include "STUSoundFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSoundFunctionLibrary, All, All);

void USTUSoundFunctionLibrary::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
    if (SoundClass)
    {
        SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.f, 1.f);
        UE_LOG(LogSoundFunctionLibrary, Display, TEXT("Sound class volume changed: %s = %f"), *SoundClass->GetName(), SoundClass->Properties.Volume);
    }
}
