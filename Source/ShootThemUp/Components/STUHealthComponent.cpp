// Fill out your copyright notice in the Description page of Project Settings.

#include "STUHealthComponent.h"
#include "TimerManager.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

#include "../Dev/STUFireDamageType.h"
#include "../Dev/STUIceDamageType.h"

#include "../STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float USTUHealthComponent::GetHealth() const
{
	return Health;
}

float USTUHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float USTUHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool USTUHealthComponent::IsDead() const
{
	return Health <= 0.f;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (!IsDead() && Health < MaxHealth)
	{
		SetHealth(Health + HealthAmount);
		return true;
	}

	return false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || IsDead()) 
		return;

	SetHealth(Health - Damage);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (bAutoHeal)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelayTime);
	}

	PlayShake();
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	const float NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const float HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta, MaxHealth);
}

void USTUHealthComponent::HealUpdate()
{
	if (!bAutoHeal || Health == MaxHealth || IsDead())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
	else
	{
		SetHealth(Health + HealModifier);
	}
}

void USTUHealthComponent::PlayShake()
{
	if (!IsDead())
	{
		const auto Player = Cast<APawn>(GetOwner());

		if (Player)
		{
			const auto Controller = Player->GetController<APlayerController>();

			if (Controller && Controller->PlayerCameraManager)
			{
				Controller->PlayerCameraManager->StartCameraShake(CameraShake);
			}
		}
	}
}

void USTUHealthComponent::Killed(AController* Killer)
{
	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode)
	{
		const auto Player = Cast<APawn>(GetOwner());
		const auto Controller = Player ? Player->Controller : nullptr;

		GameMode->Killed(Killer, Controller);
	}
}

