// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnDeath) // C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath); // C++ & BP

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged); // C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, Health, float, HealthDelta, float, MaxHealth); // C++ & BP

class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	bool TryToAddHealth(float HealthAmount);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoHeal = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AutoHeal"))
	float HealDelayTime = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AutoHeal"))
	float HealModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UPhysicalMaterial*, float> DamageModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	virtual void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );

	UFUNCTION(BlueprintCallable)
	virtual void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser );

private:
	void SetHealth(float NewHealth);
	void HealUpdate();
	
	float GetDamageModifier(AActor* DamagedActor, FName BoneName) const;
	
	void ReportDamageEvent(float Damage, AController* InvistigatedBy);
	void ApplyDamage(float Damage, AController* InstigatedBy);

	void PlayShake();
	void Killed(AController* Killer);
	
	float Health = 0.f;
	FTimerHandle HealTimerHandle;
};
