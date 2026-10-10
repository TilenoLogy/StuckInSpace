// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkManager.h"
#include "ElectricityMachineBase.h"
#include "BatteryBase.h"
#include "WireBase.h"
#include "EngineUtils.h"
#include "TimerManager.h"

// Sets default values
ANetworkManager::ANetworkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ANetworkManager::RebuildAllNetworks(UWorld* World)
{
    if (!World)
    {
        return;
    }

    TArray<AElectricityMachineBase*> Machines;
    for (TActorIterator<AElectricityMachineBase> It(World); It; ++It)
    {
        if (IsValid(*It) && !(*It)->IsActorBeingDestroyed())
        {
            Machines.Add(*It);
        }
    }

    TArray<AWireBase*> Wires;
    for (TActorIterator<AWireBase> It(World); It; ++It)
    {
        if (IsValid(*It) && !(*It)->IsActorBeingDestroyed())
        {
            Wires.Add(*It);
        }
    }

    TArray<ANetworkManager*> Managers;
    for (TActorIterator<ANetworkManager> It(World); It; ++It)
    {
        if (IsValid(*It) && !(*It)->IsActorBeingDestroyed())
        {
            Managers.Add(*It);
            It->Devices.Empty();
        }

    }

    TSet<AElectricityMachineBase*> Unassigned;
    for (AElectricityMachineBase* Machine : Machines)
    {
        Unassigned.Add(Machine);
    }
    int32 ComponentIndex = 0;

    while (Unassigned.Num() > 0)
    {
        AElectricityMachineBase* Start = *Unassigned.CreateIterator();
        Unassigned.Remove(Start);

        TArray<AElectricityMachineBase*> Component;
        TArray<AElectricityMachineBase*> Pending;
        Pending.Add(Start);

        while (Pending.Num() > 0)
        {
            AElectricityMachineBase* Current = Pending.Pop();
            Component.Add(Current);

            for (AWireBase* Wire : Wires)
            {
                if (!Wire || (Wire->FirstMachine != Current && Wire->SecondMachine != Current))
                {
                    continue;
                }

                AElectricityMachineBase* Other = Wire->FirstMachine == Current
                    ? Wire->SecondMachine
                    : Wire->FirstMachine;
                if (IsValid(Other) && Unassigned.Remove(Other))
                {
                    Pending.Add(Other);
                }
            }
        }

        ANetworkManager* Network = Managers.IsValidIndex(ComponentIndex)
            ? Managers[ComponentIndex]
            : World->SpawnActor<ANetworkManager>();
        if (Network)
        {
            Network->Devices = Component;
            for (AElectricityMachineBase* Device : Component)
            {
                Device->PowerNetwork = Network;
            }
            Network->RecalculateNetwork();

            FString ComponentDescription;
            for (AElectricityMachineBase* Device : Component)
            {
                if (!ComponentDescription.IsEmpty())
                {
                    ComponentDescription += TEXT(", ");
                }
                ComponentDescription += Device->GetName();
            }

            UE_LOG(LogTemp, Warning,
                TEXT("[ElectricityNetwork] Network%d: [%s]"),
                ComponentIndex,
                *ComponentDescription);
        }
        ++ComponentIndex;
    }

    for (int32 Index = ComponentIndex; Index < Managers.Num(); ++Index)
    {
        if (Managers[Index] && Managers[Index]->Devices.Num() == 0)
        {
            Managers[Index]->Destroy();
        }
    }

}

void ANetworkManager::RebuildAllNetworksNextTick(UWorld* World)
{
    if (!World || World->bIsTearingDown)
    {
        return;
    }

    World->GetTimerManager().SetTimerForNextTick(
        FTimerDelegate::CreateStatic(&ANetworkManager::RebuildAllNetworks, World));
}

// Called when the game starts or when spawned
void ANetworkManager::BeginPlay()
{
	Super::BeginPlay();
    RefreshDevices();
}

// Called every frame
void ANetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TransferEnergy(DeltaTime);
}


void ANetworkManager::RecalculateNetwork()
{
    TotalProduction = 0.f;
    TotalConsumption = 0.f;
    TotalCapacity = 0.f;

    for (AElectricityMachineBase* Device : Devices)
    {
        if (!IsValid(Device))
            continue;

        TotalProduction += Device->PowerProduction;
        TotalConsumption += Device->PowerConsumption;
        TotalCapacity += Device->BatteryCapacity;
    }
}

void ANetworkManager::TransferEnergy(float DeltaTime)
{
    if (DeltaTime <= 0.f)
    {
        return;
    }

    RecalculateNetwork();

    const float NetEnergyRate = TotalProduction - TotalConsumption;
    float EnergyToTransfer = FMath::Abs(NetEnergyRate) * DeltaTime;

    if (NetEnergyRate > 0.f)
    {
        for (AElectricityMachineBase* Device : Devices)
        {
            ABatteryBase* Battery = Cast<ABatteryBase>(Device);
            if (!IsValid(Battery) || EnergyToTransfer <= 0.f)
            {
                continue;
            }

            EnergyToTransfer -= Battery->AddEnergy(EnergyToTransfer);
        }
    }
    else if (NetEnergyRate < 0.f)
    {
        for (AElectricityMachineBase* Device : Devices)
        {
            ABatteryBase* Battery = Cast<ABatteryBase>(Device);
            if (!IsValid(Battery) || EnergyToTransfer <= 0.f)
            {
                continue;
            }

            EnergyToTransfer -= Battery->RemoveEnergy(EnergyToTransfer);
        }
    }

    RecalculateNetwork();
}

void ANetworkManager::RegisterDevice(AElectricityMachineBase* Device)
{
    if (!IsValid(Device))
        return;

    Devices.AddUnique(Device);
    Device->PowerNetwork = this;
    RecalculateNetwork();
}

void ANetworkManager::RefreshDevices()
{
    Devices.Empty();

    for (TActorIterator<AElectricityMachineBase> It(GetWorld()); It; ++It)
    {
        AElectricityMachineBase* Device = *It;
        if (IsValid(Device) && Device->PowerNetwork == this)
        {
            Devices.AddUnique(Device);
        }
    }

    RecalculateNetwork();
}

void ANetworkManager::MergeNetwork(ANetworkManager* NetworkA,
    ANetworkManager* NetworkB)
{
    if (!NetworkA || !NetworkB || NetworkA == NetworkB)
        return;

    NetworkA->RefreshDevices();
    NetworkB->RefreshDevices();

    for (AElectricityMachineBase* Device : NetworkB->Devices)
    {
        NetworkA->RegisterDevice(Device);
    }

    NetworkA->RecalculateNetwork();
    NetworkB->Destroy();
}

