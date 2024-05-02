// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTURespawnComponent();

	UFUNCTION(BlueprintCallable)
	void Respawn(int32 Time);

	UFUNCTION(BlueprintCallable)
	int32 GetRespawnTimeRemaining() const;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		

private:
	void RespawnUpdate();

	int32 RespawnCountDown;
	FTimerHandle RespawnTimerHandle;
};
