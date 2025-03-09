// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#include "Delegate/RockGameplayEventBinding.h"

void FRockGameplayEventBinding::BindMulticastDelegate(UObject* DelegateOwner, const FMulticastDelegateProperty* DelegateProperty) const
{
	// FindFunctionByName should have been verified at an early stage. We could consider removing it here?
	if (IsValid() && TargetActor->GetClass()->FindFunctionByName(FunctionNameToBind))
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(TargetActor, FunctionNameToBind);
		DelegateProperty->AddDelegate(MoveTemp(Delegate), DelegateOwner);
	}
}

void FRockGameplayEventBinding::BindBlueprintDelegate(UObject* DelegateOwner, const FDelegateProperty* DelegateProperty) const
{
	if (IsValid() && TargetActor->GetClass()->FindFunctionByName(FunctionNameToBind))
	{
		FScriptDelegate* ScriptDelegate = DelegateProperty->GetPropertyValuePtr_InContainer(DelegateOwner);
		ScriptDelegate->BindUFunction(TargetActor, FunctionNameToBind);
	}
}

bool FRockGameplayEventBinding::IsValid() const
{
	return TargetActor && !FunctionNameToBind.IsNone();
}

FString FRockGameplayEventBinding::ToString() const
{
	return FString::Printf(TEXT("TargetActor: %s, FunctionNameToBind: %s"), *GetNameSafe(TargetActor), *FunctionNameToBind.ToString());
}

// void FRockGameplayEventBinding::BindSparseDelegate(UObject* DelegateOwner, FName DelegateName, FSparseDelegate* SparseDelegate) const
// {
// 	// It seems like SparseDelegates can seemingly safely use the above approach and this whole function is unnecesary?
// 	
// 	// FindFunctionByName should have been verified at an earlier stage. We could consider removing it here?
// 	if (TargetActor && TargetActor->GetClass()->FindFunctionByName(FunctionNameToBind))
// 	{
// 		FScriptDelegate Delegate;
// 		Delegate.BindUFunction(TargetActor, FunctionNameToBind);
// 		SparseDelegate->__Internal_AddUnique(DelegateOwner, DelegateName, MoveTemp(Delegate));
//
// 		// Do we care about non multicast for this plugin?
// 		// If instead we were using FMulticastSparseDelegateProperty
// 		// FMulticastSparseDelegateProperty* MulticastSparseDelegateProperty;// = CastField<FMulticastSparseDelegateProperty>(DelegateOwner->GetClass()->FindPropertyByName(DelegateName));
// 		// MulticastSparseDelegateProperty->AddDelegate(MoveTemp(Delegate), DelegateOwner);
// 		
// 	}
// }

// if (const FDelegateProperty* CastProp = CastField<FDelegateProperty>(Prop))
// {
// 	const FScriptDelegate* Value = CastProp->GetPropertyValuePtr(ValueAddr);
// 	OutPyObj = (PyObject*)FPyWrapperDelegateFactory::Get().CreateInstance(CastProp->SignatureFunction, (FScriptDelegate*)Value, OwnerContext, ConversionMethod);
// 	return FPyConversionResult::Success();
// }
//
// if (const FMulticastDelegateProperty* CastProp = CastField<FMulticastDelegateProperty>(Prop))
// {
// 	const FMulticastScriptDelegate* Value = CastProp->GetMulticastDelegate(ValueAddr);
// 	OutPyObj = (PyObject*)FPyWrapperMulticastDelegateFactory::Get().CreateInstance(CastProp->SignatureFunction, (FMulticastScriptDelegate*)Value, OwnerContext, ConversionMethod);
// 	return FPyConversionResult::Success();
// }


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