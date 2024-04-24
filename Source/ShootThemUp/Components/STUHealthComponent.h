// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnDeath) // C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath); // C++ & BP

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged); // C++
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Health, float, MaxHealth); // C++ & BP

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

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoHeal = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AutoHeal"))
	float HealDelayTime = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AutoHeal"))
	float HealModifier = 1.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	void HealUpdate();
	
	float Health = 0.f;
	FTimerHandle HealTimerHandle;
};
