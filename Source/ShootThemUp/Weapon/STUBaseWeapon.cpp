// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

