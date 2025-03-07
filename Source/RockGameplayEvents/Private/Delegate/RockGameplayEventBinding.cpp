// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.


#include "Delegate/RockGameplayEventBinding.h"

void FRockGameplayEventBinding::BindDelegate(UObject* DelegateOwner, const FMulticastDelegateProperty* DelegateProperty) const
{
	// FindFunctionByName should have been verified at an early stage. We could consider removing it here?
	if (TargetActor && TargetActor->GetClass()->FindFunctionByName(FunctionNameToBind))
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(TargetActor, FunctionNameToBind);
		DelegateProperty->AddDelegate(MoveTemp(Delegate), DelegateOwner);
	}
}

void FRockGameplayEventBinding::BindSparseDelegate(UObject* DelegateOwner, FName DelegateName, FSparseDelegate* SparseDelegate) const
{
	// FindFunctionByName should have been verified at an earlier stage. We could consider removing it here?
	if (TargetActor && TargetActor->GetClass()->FindFunctionByName(FunctionNameToBind))
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(TargetActor, FunctionNameToBind);
		SparseDelegate->__Internal_AddUnique(DelegateOwner, DelegateName, MoveTemp(Delegate));
	}
}



// FMulticastScriptDelegate*
// BindInlineDelegate
// ResolveDelegateReference(this, Parent, PropertyValue);
//      	FMulticastScriptDelegate& MulticastDelegate = (*(FMulticastScriptDelegate*)PropertyValue);
//      	// Add this delegate to our multicast delegate's invocation list
//      	MulticastDelegate.AddUnique(MoveTemp(ScriptDelegate));

// ResolveDelegateReference(this, Parent, PropertyValue);
// USparseDelegateFunction* SparseDelegateFunc = CastChecked<USparseDelegateFunction>(SignatureFunction);
// FSparseDelegate& SparseDelegate = *(FSparseDelegate*)PropertyValue;
// SparseDelegate.__Internal_AddUnique(Parent, SparseDelegateFunc->DelegateName, MoveTemp(ScriptDelegate));