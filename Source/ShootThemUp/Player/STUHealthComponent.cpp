// Fill out your copyright notice in the Description page of Project Settings.

#include "STUHealthComponent.h"

#include "../Dev/STUFireDamageType.h"
#include "../Dev/STUIceDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float USTUHealthComponent::GetHealth() const
{
	return Health;
}

bool USTUHealthComponent::IsDead() const
{
	return Health <= 0.f;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);

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

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (IsDead())
		OnDeath.Broadcast();

    //if (DamageType)
    //{
    //	if (DamageType->IsA<USTUFireDamageType>())
    //	{
    //		UE_LOG(LogHealthComponent, Display, TEXT("So hooooooot!"));
    //	}
    //	else if (DamageType->IsA<USTUIceDamageType>())
    //	{
    //		UE_LOG(LogHealthComponent, Display, TEXT("So coooooold!"));
    //	}
    //}
}

