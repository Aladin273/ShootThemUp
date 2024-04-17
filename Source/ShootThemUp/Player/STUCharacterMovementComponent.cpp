// Fill out your copyright notice in the Description page of Project Settings.


#include "STUCharacterMovementComponent.h"
#include "STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    return IsRunning() ? MaxWalkSpeedRunning : MaxSpeed;
}

bool USTUCharacterMovementComponent::IsRunning() const
{
    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return WantsToRun && Character->GetInputVelocity().X > 0 && !Character->GetVelocity().IsZero();
}

void USTUCharacterMovementComponent::StartRunning()
{
    WantsToRun = true;
}

void USTUCharacterMovementComponent::StopRunning()
{
    WantsToRun = false;
}
