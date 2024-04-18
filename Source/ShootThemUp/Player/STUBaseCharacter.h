// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTextRenderComponent;
class USTUHealthComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FallAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DeathLifeSpan = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float FallDelay = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector2D FallDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector2D FallDamageVelocity = FVector2D(800.f, 1600.f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	UFUNCTION(BlueprintCallable)
	virtual FVector GetInputVelocity() const;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetRelativeVelocity() const;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

	UFUNCTION(BlueprintCallable)
	virtual void OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	virtual void OnLand(const FHitResult& Hit);

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void LookUp(float Amount);
	void LookAround(float Amount);

	FVector InputVelocity = FVector::ZeroVector;
};
