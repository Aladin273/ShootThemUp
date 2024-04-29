// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAICharacter.h"
#include "STUAIController.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "../Components/STUAIWeaponComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
    }
}

UBehaviorTree* ASTUAICharacter::GetBehaviorTreeAsset() const
{
    return BehaviorTreeAsset;
}