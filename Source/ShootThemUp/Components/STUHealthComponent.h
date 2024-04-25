// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnDeath) // C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath); // C++ & BP

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged); // C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, Health, float, HealthDelta, float, MaxHealth); // C++ & BP

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	void SetHealth(float NewHealth);
	void HealUpdate();
	
	void PlayShake();
	
	float Health = 0.f;
	FTimerHandle HealTimerHandle;
};
