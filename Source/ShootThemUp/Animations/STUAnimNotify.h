// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*, MeshComp); // C++ & BP

template<typename T>
T* FindNotifyByClass(UAnimSequenceBase* Animation)
{
	auto NotifyEvent = Animation->Notifies.FindByPredicate([](auto Event) { return Cast<T>(Event.Notify); });
	return NotifyEvent ? Cast<T>(NotifyEvent->Notify) : nullptr;
}

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnNotifiedSignature OnNotified;
};
