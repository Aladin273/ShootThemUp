// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Weapon/STUBaseWeapon.h"
#include "STUWeaponComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	bool IsFiring() const;

	UFUNCTION(BlueprintCallable)
	void NextWeapon();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	FWeaponUIData GetUIData() const;

	UFUNCTION(BlueprintCallable)
	FWeaponAmmoData GetAmmoData() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<FWeaponData> WeaponData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponEquipSocket = "WeaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponArmorySocket = "ArmorySocket";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bAutoReload = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	struct FWeaponDataInternal
	{
		ASTUBaseWeapon* Weapon;
		UAnimMontage* ReloadAnimMontage;
	};

	TArray<FWeaponDataInternal> Weapons;
	FWeaponDataInternal CurrentWeapon;	
	int32 CurrentWeaponIndex = 0;
};