// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class APlayerController;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmptyClipSignature); // C++ & BP

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	UTexture2D* MainIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
	UTexture2D* CrossHairIcon;
};

USTRUCT(BlueprintType)
struct FWeaponAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	int32 Bullets = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
	int32 Clips = 3;

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
	FWeaponUIData GetUIData() const;

	UFUNCTION(BlueprintCallable)
	FWeaponAmmoData GetAmmoData() const;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEmptyClipSignature OnEmptyClip;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocket = "MuzzleSocket";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	FWeaponUIData UIData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	FWeaponAmmoData AmmoData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	float TraceMaxDistance = 10000.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void LogAmmo();

	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);
	
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
	FVector GetMuzzleWorldLocation() const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

private:
	FWeaponAmmoData CurrentAmmo;
};
