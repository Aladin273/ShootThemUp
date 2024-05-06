// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAICharacter.h"
#include "STUAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

#include "Components/WidgetComponent.h"
#include "../UI/STUHealthBarWidget.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

UBehaviorTree* ASTUAICharacter::GetBehaviorTreeAsset() const
{
    return BehaviorTreeAsset;
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    AAIController* AIController = Cast<AAIController>(Controller);

    if (AIController && AIController->BrainComponent)
        AIController->BrainComponent->Cleanup();
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta, float MaxHealth)
{
    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());

    if (HealthBarWidget)
    {
        HealthBarWidget->ColorDefault = GetPlayerColor();
        HealthBarWidget->bUsePercentBasedColor = false;
        
        HealthBarWidget->SetHealthPercent(Health / MaxHealth);
    }
}
