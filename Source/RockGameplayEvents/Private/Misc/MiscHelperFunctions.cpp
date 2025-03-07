// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Misc/MiscHelperFunctions.h"

#include "Kismet2/BlueprintEditorUtils.h"

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

		if (FMulticastDelegateProperty* DelegateProperty = CastField<FMulticastDelegateProperty>(Property))
		{
			FRockDelegateInfo Info;
			Info.Name = DelegateProperty->GetName();
			if (DelegateProperty->GetOwnerClass())
			{
				Info.DefiningClass = DelegateProperty->GetOwnerClass();
			}
			Info.bIsMulticast = true;
			Info.bIsSparse = DelegateProperty->GetClass()->HasAnyCastFlags(CASTCLASS_FMulticastSparseDelegateProperty);
			// HasAnyCastFlags(CASTCLASS_FDelegateProperty | CASTCLASS_FMulticastDelegateProperty | CASTCLASS_FMulticastInlineDelegateProperty | CASTCLASS_FMulticastSparseDelegateProperty)
			
			if (DelegateProperty->SignatureFunction)
			{
				Info.SignatureFunction = DelegateProperty->SignatureFunction;
			}

			DelegateInfos.Add(Info);
		}
		else if (FDelegateProperty* SingleDelegateProperty = CastField<FDelegateProperty>(Property))
		{
			FRockDelegateInfo Info;
			Info.Name = SingleDelegateProperty->GetName();
			if (DelegateProperty->GetOwnerClass())
			{
				Info.DefiningClass = DelegateProperty->GetOwnerClass();
			}
			Info.bIsMulticast = false;
			Info.bIsSparse = DelegateProperty->GetClass()->HasAnyCastFlags(CASTCLASS_FMulticastSparseDelegateProperty);
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


void UMiscHelperFunctions::PrintDelegateInfo(TArray<FRockDelegateInfo> InDelegates)
{
	for (FRockDelegateInfo Info : InDelegates)
	{
		FString MulticastString = Info.bIsMulticast ? TEXT("[Multi]") : TEXT("[Single]");
		FString SparseString = Info.bIsSparse ? TEXT("[Sparse]") : TEXT("");
		UE_LOG(LogTemp, Warning, TEXT("%s::%s%s%s, Signature: %s"),
			*Info.DefiningClass->GetName(), *Info.Name, *MulticastString, *SparseString, *Info.SignatureFunction->GetName());
	}
}

TArray<FRockFunctionInfo> UMiscHelperFunctions::GetFunctionsForActor(AActor* InActor)
{
	TArray<FRockFunctionInfo> FunctionInfos;
	if (!InActor)
	{
		return FunctionInfos;
	}

	UClass* ActorClass = InActor->GetClass();
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

void UMiscHelperFunctions::PrintFunctionInfo(TArray<FRockFunctionInfo> InFunctions)
{
	for (FRockFunctionInfo Info : InFunctions)
	{
		FString EventString = Info.bIsEvent ? TEXT("Event_") : TEXT("");
		FString NativeString = Info.bIsNative ? TEXT("Native") : TEXT("BP");
		FString RPCString = Info.bIsRPC ? TEXT("[RPC]") : TEXT("");
		FString DefiningClassString = Info.DefiningClass ? Info.DefiningClass->GetName() : TEXT("");
		// Print all the parameters on 1 UE_Log line
		FString ParamString = BuildFunctionParameterString(Info.Function);
		UE_LOG(LogTemp, Warning, TEXT("Function: %s::%s%s::%s%s (%s)"), *DefiningClassString, *EventString, *NativeString, *Info.Name, *RPCString,
			*ParamString);
	}
}

FString UMiscHelperFunctions::BuildFunctionParameterString(UFunction* InFunction, bool bIncludeParameterType, bool bIncludeParameterName, bool bIncludeParameterFlags)
{
	FString ParamString = TEXT("");
	if (!InFunction)
	{
		return ParamString;
	}
	for (TFieldIterator<FProperty> ParamIt(InFunction); ParamIt; ++ParamIt)
	{
		// We could add more info like the Type instead, which is what we need for compability purposes.
		FProperty* Param = *ParamIt;
		FString ParamInfo;
			
		// TODO: Move these to UDeveloperSettings
		if (bIncludeParameterName)
		{
			ParamInfo = Param->GetName() + TEXT(" : ");
		}
		if (bIncludeParameterType)
		{
			ParamInfo += Param->GetCPPType();
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

bool UMiscHelperFunctions::CanFunctionBindToDelegate(UFunction* InFunction, UFunction* DelegateFunction)
{
	// UBlueprintBoundEventNodeSpawner::IsBindingCompatible
	const bool bMatchingThreadSafeness = FBlueprintEditorUtils::HasFunctionBlueprintThreadSafeMetaData(InFunction) ==
		FBlueprintEditorUtils::HasFunctionBlueprintThreadSafeMetaData(DelegateFunction);
	const bool bIsSignatureCompatible = InFunction->IsSignatureCompatibleWith(DelegateFunction);

	return bIsSignatureCompatible && bMatchingThreadSafeness;
}

TArray<FRockFunctionInfo> UMiscHelperFunctions::GetCompatibleFunctionsForDelegate(AActor* InActor, FRockDelegateInfo DelegateProperty)
{
	TArray<FRockFunctionInfo> CompatibleFunctions;
	UFunction* DelegateSignature = DelegateProperty.SignatureFunction;

	if (!InActor || !DelegateSignature)
	{
		return CompatibleFunctions;
	}
	auto ActorFunctions = GetFunctionsForActor(InActor);
	for (FRockFunctionInfo FuncInfo : ActorFunctions)
	{
		if (CanFunctionBindToDelegate(FuncInfo.Function, DelegateSignature))
		{
			CompatibleFunctions.Add(FuncInfo);
		}
	}
	return CompatibleFunctions;
}
