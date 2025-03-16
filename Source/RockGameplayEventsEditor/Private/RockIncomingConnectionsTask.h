// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "GameFramework/Actor.h"

class URockDelegateConnectorComponent;

class FRockIncomingConnectionsTask : public FNonAbandonableTask
{
public:
	FRockIncomingConnectionsTask(TWeakObjectPtr<AActor> InActor, TWeakObjectPtr<URockDelegateConnectorComponent> InConnector)
		: WeakActor(InActor),
		  WeakConnector(InConnector)
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
