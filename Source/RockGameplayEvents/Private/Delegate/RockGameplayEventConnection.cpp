// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventConnection.h"

#include "Misc/MiscHelperFunctions.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void FRockGameplayEventConnection::Connect(UObject* DelegateOwner, const UClass* SourceClass)
{
	if (!DelegateOwner || !SourceClass)
	{
		// UE_CLOG? Conditional logging
		UE_LOG(LogRockGameplayEvents, Error, TEXT("Actor or SourceClass is null"));
		return;
	}

	// Instead of a switch
	// Perhaps we just hold a pointer to the 2 FDelegateProperty and FMulticastDelegateProperty
	// the PropertyName(12 bytes) + type (1byte) +3 padding= >16.  But 2 pointers is 16 bytes
	// So size wise they are identical for now
	// But we save the 'computational time' of having to do FindFProperty, which I'm sure is O(n) where n is the FPropertys in the class? 

	switch (DelegateType)
	{
	case ERockDelegateType::MulticastDelegate:
		{
			// Could we cache this during the initial setup? instead of doing this at Connect time?
			const FMulticastDelegateProperty* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(SourceClass, DelegatePropertyName);
			if (DelegateProperty)
			{
				for (FRockGameplayEventBinding connection : Bindings)
				{
					connection.BindMulticastDelegate(DelegateOwner, DelegateProperty);
				}
			}
			break;
		}
	case ERockDelegateType::BlueprintDelegate:
		{
			FDelegateProperty* DelegateProperty = FindFProperty<FDelegateProperty>(SourceClass, DelegatePropertyName);
			if (DelegateProperty)
			{
				for (FRockGameplayEventBinding connection : Bindings)
				{
					connection.BindBlueprintDelegate(DelegateOwner, DelegateProperty);
				}
			}
			break;
		}
	default:
		{
			UE_LOG(LogRockGameplayEvents, Error, TEXT("DelegateType not implemented"));
			break;
		}
	}
}

FString FRockGameplayEventConnection::GetDelegateNameString() const
{
	return DelegatePropertyName.ToString();
}

FString FRockGameplayEventConnection::ToString() const
{
	return FString::Printf(TEXT("Delegate: %s, Bindings: %d"), *DelegatePropertyName.ToString(), Bindings.Num());
}

#if WITH_EDITOR
EDataValidationResult FRockGameplayEventConnection::IsDataValid(class FDataValidationContext& Context) const
{
	for (int i = 0; i < Bindings.Num(); i++)
	{
		if (!Bindings[i].IsValid())
		{
			Context.AddWarning(FText::Format(NSLOCTEXT("GameplayEventConnection", "InvalidBinding", "Invalid binding at index {1}"),
				FText::FromName(DelegatePropertyName), i));
			return EDataValidationResult::Invalid;
		}
	}
	return EDataValidationResult::Valid;
}
#endif

// It's unclear to me, but Inline and Sparse seems to successfully bind to the above Delegate
// Even in the PyConversion.cpp,  they only handle FMulticastDelegateProperty and FDelegateProperty
// Though they do have a 'TODO' about SparseDelegateProperty?
// I think it's because all 3 (Multicast, Sparse, and Inline) of them override the same AddDelegate? 


// case ERockDelegateType::Sparse:
// 	{
// 		// ResolveSparseDelegate WILL check(false) so we MUST return before reaching it, if it's not a SparseDelegate
// 		FSparseDelegate* SparseDelegate = FSparseDelegateStorage::ResolveSparseDelegate(Actor, DelegatePropertyName);
// 		if (SparseDelegate)
// 		{
// 			for (FRockGameplayEventBinding connection : Bindings)
// 			{
// 				UE_LOG(LogRockGameplayEvents, Warning, TEXT("SparseDelegate"));
// 				connection.BindSparseDelegate(Actor, DelegatePropertyName, SparseDelegate);
// 			}
// 		}
// 		break;
// 	}


// TODO thought?
//{
// DelegateProperty->GetMulticastDelegate();
// Do I Want FMulticastScriptDelegate or FMulticastDelegateProperty ???????
//FMulticastScriptDelegate* DelegateTest = nullptr;
//DelegateTest->Add(Delegate);
//}
// Is there a difference here between BlueprintDelegate and a MulticastDelegate?
// class FMulticastDelegateProperty;
// class FMulticastSparseDelegateProperty;
// class FMulticastInlineDelegateProperty;
// ?? FMulticastScriptDelegate
