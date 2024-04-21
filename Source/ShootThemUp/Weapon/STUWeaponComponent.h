// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponDataInternal
{
	GENERATED_USTRUCT_BODY()

	ASTUBaseWeapon* Weapon;
	UAnimMontage* ReloadAnimMontage;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<FWeaponData> WeaponData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponEquipSocket = "WeaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponArmorySocket = "ArmorySocket";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void NextWeapon();

	UFUNCTION(BlueprintCallable)
	void Reload();

private:
	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	void InitAnimations();
	void SpawnWeapons();
	void EquipWeapon(int32 Index);
	void AttachToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, FName SocketName);

	void PlayAnimMontage(UAnimMontage* AnimMontage);

	UFUNCTION()
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	bool bEquipAnimInProgress = false;

	UFUNCTION()
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);
	bool bReloadAnimInProgress = false;

	UPROPERTY()
	TArray<FWeaponDataInternal> Weapons;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage;

	int32 CurrentWeaponIndex = 0;
};