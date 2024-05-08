// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class APlayerController;
class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmptyClipSignature, ASTUBaseWeapon*, Weapon); // C++ & BP

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	UTexture2D* MainIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UTexture2D* CrossHairIcon;
};

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	int32 Bullets = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
	int32 Stock = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	bool bInfinite = false;
};

UCLASS(Blueprintable)
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	ASTUBaseWeapon();

public:
	UFUNCTION(BlueprintCallable)
	virtual void StartFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();
	
	UFUNCTION(BlueprintCallable)
	virtual bool IsFiring() const;

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	bool CanReload() const;

	UFUNCTION(BlueprintCallable)
	bool IsAmmoFull() const;

	UFUNCTION(BlueprintCallable)
	bool IsAmmoEmpty() const;

	UFUNCTION(BlueprintCallable)
	bool IsClipEmpty() const;

	UFUNCTION(BlueprintCallable)
	FWeaponUIData GetUIData() const;

	UFUNCTION(BlueprintCallable)
	FWeaponAmmoData GetAmmoData() const;

	UFUNCTION(BlueprintCallable)
	bool TryToAddAmmo(int32 Bullets);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEmptyClipSignature OnEmptyClip;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocket = "MuzzleSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	float TraceMaxDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	FWeaponUIData UIData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	FWeaponAmmoData AmmoData;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX") 
	UNiagaraSystem* MuzzleFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void DecreaseAmmo();
	void LogAmmo();

	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);
	
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
	FVector GetMuzzleWorldLocation() const;

	AController* GetController() const;
	bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	UNiagaraComponent* SpawnMuzzleFX();
	UNiagaraComponent* SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

private:
	FWeaponAmmoData CurrentAmmo;
};
