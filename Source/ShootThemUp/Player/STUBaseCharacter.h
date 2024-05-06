// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUCharacterMovementComponent;
class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

	UFUNCTION(BlueprintCallable)
	FLinearColor GetPlayerColor() const;

	UFUNCTION(BlueprintCallable)
	void SetPlayerColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable)
    virtual FVector GetMovementInput() const;

	UFUNCTION(BlueprintCallable)
    virtual FRotator GetRotationInput() const;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetRelativeVelocity() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FallAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DeathLifeSpan = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float FallDelay = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector2D FallDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector2D FallDamageVelocity = FVector2D(800.f, 1600.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FName MaterialColorName = "Paint Color";

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

	UFUNCTION(BlueprintCallable)
	virtual void OnHealthChanged(float Health, float HealthDelta, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	virtual void OnLand(const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	FLinearColor PlayerColor;
	FVector MovementInput = FVector::ZeroVector;
	USTUCharacterMovementComponent* CharacterMovementComponent = nullptr;
};
