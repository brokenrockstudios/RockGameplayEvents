// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RockDelegateConnectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FRockGameplayEventBinding
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> TargetActor;

	// This must match the signature of FRockGameplayEventConnection::DelegatePropertyName
	UPROPERTY(EditAnywhere)
	FName FunctionNameToBind; 
};

USTRUCT(BlueprintType)
struct FRockGameplayEventConnection
{
	GENERATED_BODY()
	// The owner's Multicast Delegate Property to bind to
	UPROPERTY(EditAnywhere)
	FName DelegatePropertyName;
	
	UPROPERTY(EditAnywhere)
	TArray<FRockGameplayEventBinding> Bindings;

private:
	friend class URockDelegateConnectorComponent;
	void Connect(const UClass* SourceClass);
};


UCLASS(ClassGroup=(RockGameplayEvents), meta=(BlueprintSpawnableComponent), HideCategories = (Activation, Navigation, Tags, Cooking, AssetUserData))
class ROCKGAMEPLAYEVENTS_API URockDelegateConnectorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	URockDelegateConnectorComponent();

	UPROPERTY(EditAnywhere, Category = "Delegates")
	TArray<FRockGameplayEventConnection> DelegateConnections;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
