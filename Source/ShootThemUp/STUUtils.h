#pragma once

template <typename T>
static T* GetSTUPlayerComponent(APawn* PlayerPawn)
{
    return PlayerPawn ? Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass())) : nullptr;
}