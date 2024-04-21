// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);
    
    Super::Notify(MeshComp, Animation, EventReference);
}