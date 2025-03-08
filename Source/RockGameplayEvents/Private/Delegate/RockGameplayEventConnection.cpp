// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventConnection.h"

void FRockGameplayEventConnection::Connect(AActor* Actor, const UClass* SourceClass)
{
	if (!Actor || !SourceClass)
	{
		// UE_CLOG? Conditional logging
		UE_LOG(LogTemp, Error, TEXT("Actor or SourceClass is null"));
		return;
	}

	switch (DelegateType)
	{
	case ERockDelegateType::Regular:
		{
			// Could we cache this during the initial setup? instead of doing this at Connect time?
			const FMulticastDelegateProperty* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(SourceClass, DelegatePropertyName);
			if (DelegateProperty)
			{
				for (FRockGameplayEventBinding connection : Bindings)
				{
					connection.BindDelegate(Actor, DelegateProperty);
				}
				return;
			}
			break;
		}
	case ERockDelegateType::Sparse:
		{
			// ResolveSparseDelegate WILL check(false) so we MUST return before reaching it, if it's not a SparseDelegate
			FSparseDelegate* SparseDelegate = FSparseDelegateStorage::ResolveSparseDelegate(Actor, DelegatePropertyName);
			if (SparseDelegate)
			{
				for (FRockGameplayEventBinding connection : Bindings)
				{
					connection.BindSparseDelegate(Actor, DelegatePropertyName, SparseDelegate);
				}
				return;
			}
			break;
		}
	}
}



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