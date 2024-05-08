// Fill out your copyright notice in the Description page of Project Settings.

#include "STUWeaponFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/DecalComponent.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
	auto ImpactData = DefaultImpactData;

	if (HitResult.PhysMaterial.IsValid())
	{
		const auto PhysMat = HitResult.PhysMaterial.Get();

		if (ImpactDataMap.Contains(PhysMat))
			ImpactData = ImpactDataMap[PhysMat];
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, HitResult.Location, HitResult.ImpactNormal.Rotation());
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactData.SoundEffect, HitResult.ImpactPoint);

	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalData.Material, ImpactData.DecalData.Size, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
		DecalComponent->SetFadeScreenSize(ImpactData.DecalData.FadeScreenSize);
	}
}

void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTUWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

