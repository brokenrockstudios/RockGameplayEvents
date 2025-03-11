// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Misc/MiscHelperFunctions.h"

#include "Component/RockDelegateConnectorComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogRockGameplayEvents);

TArray<FRockDelegateInfo> UMiscHelperFunctions::GetDelegatesForActor(AActor* InActor)
{
	if (!InActor)
	{
		return {};
	}
	const UClass* ActorClass = InActor->GetClass();
	return GetDelegatesForActorClass(ActorClass);
}

TArray<FRockDelegateInfo> UMiscHelperFunctions::GetDelegatesForActorClass(const UClass* ActorClass)
{
	TArray<FRockDelegateInfo> DelegateInfos;
	for (TFieldIterator<FProperty> PropIt(ActorClass, EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (const FMulticastDelegateProperty* MulticastDelegateProperty = CastField<FMulticastDelegateProperty>(Property))
		{
			FRockDelegateInfo Info;
			Info.Name = MulticastDelegateProperty->GetName();
			if (MulticastDelegateProperty->GetOwnerClass())
			{
				Info.DefiningClass = MulticastDelegateProperty->GetOwnerClass();
			}
			Info.DelegateType = ERockDelegateType::MulticastDelegate;
			if (MulticastDelegateProperty->SignatureFunction)
			{
				Info.SignatureFunction = MulticastDelegateProperty->SignatureFunction;
			}
			DelegateInfos.Add(Info);
		}
		else if (const FDelegateProperty* SingleDelegateProperty = CastField<FDelegateProperty>(Property))
		{
			FRockDelegateInfo Info;
			Info.Name = SingleDelegateProperty->GetName();
			if (SingleDelegateProperty->GetOwnerClass())
			{
				Info.DefiningClass = SingleDelegateProperty->GetOwnerClass();
			}
			Info.DelegateType = ERockDelegateType::BlueprintDelegate;
			if (SingleDelegateProperty->SignatureFunction)
			{
				Info.SignatureFunction = SingleDelegateProperty->SignatureFunction;
			}
			DelegateInfos.Add(Info);
		}
	}

	DelegateInfos.Sort([](const FRockDelegateInfo& A, const FRockDelegateInfo& B)
	{
		const UClass* AClass = A.DefiningClass;
		const UClass* BClass = B.DefiningClass;
		if (!AClass || !BClass)
		{
			return false;
		}
		if (AClass == BClass)
		{
			return A.Name < B.Name;
		}
		return A.DefiningClass->IsChildOf(B.DefiningClass);
	});
	return DelegateInfos;
}

TArray<FRockFunctionInfo> UMiscHelperFunctions::GetFunctionsForActor(AActor* InActor)
{
	TArray<FRockFunctionInfo> FunctionInfos;
	if (!InActor)
	{
		return FunctionInfos;
	}

	const UClass* ActorClass = InActor->GetClass();
	for (TFieldIterator<UFunction> FuncIt(ActorClass); FuncIt; ++FuncIt)
	{
		UFunction* Function = *FuncIt;

		FRockFunctionInfo Info;

		Info.Name = Function->GetName();
		Info.Flags = Function->FunctionFlags;
		Info.Function = Function;
		if (Function->GetOwnerClass())
		{
			Info.DefiningClass = Function->GetOwnerClass();
		}

		// Determine function type based on flags
		Info.bIsEvent = (Function->FunctionFlags & FUNC_Event) != 0;
		Info.bIsNative = (Function->FunctionFlags & FUNC_Native) != 0;
		Info.bIsRPC = (Function->FunctionFlags & FUNC_Net) != 0;

		FunctionInfos.Add(Info);
	}

	FunctionInfos.Sort([](const FRockFunctionInfo& A, const FRockFunctionInfo& B)
	{
		const UClass* AClass = A.DefiningClass;
		const UClass* BClass = B.DefiningClass;
		if (!AClass || !BClass)
		{
			return false;
		}
		if (AClass == BClass)
		{
			return A.Name < B.Name;
		}
		return A.DefiningClass->IsChildOf(B.DefiningClass);
	});
	return FunctionInfos;
}

void UMiscHelperFunctions::LogFunctionInfo(TArray<FRockFunctionInfo> InFunctions)
{
	for (FRockFunctionInfo Info : InFunctions)
	{
		FString EventString = Info.bIsEvent ? TEXT("Event_") : TEXT("");
		FString NativeString = Info.bIsNative ? TEXT("Native") : TEXT("BP");
		FString RPCString = Info.bIsRPC ? TEXT("[RPC]") : TEXT("");
		FString DefiningClassString = Info.DefiningClass ? Info.DefiningClass->GetName() : TEXT("");
		FString ParamString = Info.GetSignatureFunctionString();
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("Function: %s::%s%s::%s%s %s"),
			*DefiningClassString, *EventString, *NativeString, *Info.Name, *RPCString, *ParamString);
	}
}

void UMiscHelperFunctions::LogDelegateInfo(TArray<FRockDelegateInfo> InDelegates)
{
	for (FRockDelegateInfo Info : InDelegates)
	{
		FString Name = Info.GetNameWithClass() + Info.GetDelegateTypeString();
		UE_LOG(LogRockGameplayEvents, Warning, TEXT("%s %s, Signature: %s"),
			*Info.GetNameWithClass(), *Info.GetDelegateTypeString(), *Info.SignatureFunction->GetName());
	}
}

FString UMiscHelperFunctions::BuildFunctionParameterString(
	UFunction* InFunction, bool bIncludeParameterType, bool bIncludeParameterName, bool bIncludeParameterFlags)
{
	FString ParamString = TEXT("");
	if (!InFunction)
	{
		return ParamString;
	}
	// If they are all false, don't add a bunch of ","
	if (bIncludeParameterType == false && bIncludeParameterName == false && bIncludeParameterFlags == false)
	{
		return ParamString;
	}
	for (TFieldIterator<FProperty> ParamIt(InFunction); ParamIt; ++ParamIt)
	{
		// We could add more info like the Type instead, which is what we need for compability purposes.
		FProperty* Param = *ParamIt;

		// TODO: Move these to UDeveloperSettings
		if (bIncludeParameterName)
		{
			ParamString += Param->GetName() + TEXT(":");
		}
		if (bIncludeParameterType)
		{
			ParamString += Param->GetCPPType();
		}
		if (bIncludeParameterFlags)
		{
			// Check if this is a return parameter
			if (Param->PropertyFlags & CPF_ReturnParm)
			{
				ParamString += TEXT(" (Return)");
			}
			// Check if this is an output parameter
			else if (Param->PropertyFlags & CPF_OutParm)
			{
				ParamString += TEXT(" (Out)");
			}
		}
		ParamString += ", ";
	}
	ParamString.RemoveFromEnd(", ");

	return ParamString;
}

void UMiscHelperFunctions::AddDelegateConnectorComponent(AActor* InActor)
{
	if (!InActor)
	{
		return;
	}
	URockDelegateConnectorComponent* ConnectorComponent = InActor->GetComponentByClass<URockDelegateConnectorComponent>();
	if (!ConnectorComponent)
	{
		ConnectorComponent = NewObject<URockDelegateConnectorComponent>(InActor);
		ConnectorComponent->RegisterComponent();
		InActor->AddInstanceComponent(ConnectorComponent);
	}
}
