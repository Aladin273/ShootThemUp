// Fill out your copyright notice in the Description page of Project Settings.

#include "STUWeaponFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
	auto Effect = DefaultEffect;

	if (HitResult.PhysMaterial.IsValid())
	{
		const auto PhysMat = HitResult.PhysMaterial.Get();

		if (EffectsMap.Contains(PhysMat))
			Effect = EffectsMap[PhysMat];
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.Location, HitResult.ImpactNormal.Rotation());
}

void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTUWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

