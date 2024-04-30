// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_PickupCanBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "../../Pickups/STUBasePickup.h"

UEnvQueryTest_PickupCanBeTaken::UEnvQueryTest_PickupCanBeTaken(const FObjectInitializer& ObjInit)
    : Super(ObjInit)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCanBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    bool bWantsBeTakeable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
        const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);

        if (PickupActor)
        {
            It.SetScore(TestPurpose, FilterType, PickupActor->CanBeTaken(), bWantsBeTakeable);
        }
    }
}
