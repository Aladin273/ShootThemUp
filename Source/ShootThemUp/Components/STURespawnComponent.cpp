// Fill out your copyright notice in the Description page of Project Settings.


#include "STURespawnComponent.h"
#include "../STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(int32 Time)
{
	RespawnCountDown = Time;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnUpdate, 1.0f, true);
}

int32 USTURespawnComponent::GetRespawnTimeRemaining() const
{
	return RespawnCountDown;
}

void USTURespawnComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USTURespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTURespawnComponent::RespawnUpdate()
{
	if (--RespawnCountDown <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->RespawnRequest(Cast<AController>(GetOwner()));
		}
	}
}

