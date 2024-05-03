// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

public:
	ASTUAICharacter(const FObjectInitializer& ObjInit);

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviorTreeAsset() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void OnDeath() override;
};
