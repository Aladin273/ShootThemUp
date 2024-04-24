// Fill out your copyright notice in the Description page of Project Settings.


#include "STUCharacterMovementComponent.h"
#include "../Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    return IsRunning() ? MaxWalkSpeedRunning : MaxSpeed;
}

bool USTUCharacterMovementComponent::IsRunning() const
{
    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return bWantsToRun && Character->GetInputVelocity().X > 0 && !Character->GetVelocity().IsZero() && !IsFalling();
}

void USTUCharacterMovementComponent::StartRunning()
{
    bWantsToRun = true;
}

void USTUCharacterMovementComponent::StopRunning()
{
    bWantsToRun = false;
}
