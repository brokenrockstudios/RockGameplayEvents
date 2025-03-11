// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class URockDelegateConnectorComponent;

class FRockIncomingConnectionsTask : public FNonAbandonableTask
{
public:
	FRockIncomingConnectionsTask(TWeakObjectPtr<AActor> InActor, TWeakObjectPtr<URockDelegateConnectorComponent> InConnector)
		: WeakActor(InActor)
		, WeakConnector(InConnector)
	{
	}

	// Required by UE4 task system
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FRockIncomingConnectionsTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	// This is the actual task function that will run on a worker thread
	void DoWork();

private:
	TWeakObjectPtr<AActor> WeakActor;
	TWeakObjectPtr<URockDelegateConnectorComponent> WeakConnector;
};
