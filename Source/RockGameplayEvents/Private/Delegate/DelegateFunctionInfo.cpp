#include "Delegate/DelegateFunctionInfo.h"

FString FRockDelegateInfo::GetName() const
{
	return Name;
}

FString FRockDelegateInfo::GetNameWithClass() const
{
	return (DefiningClass ? DefiningClass->GetName() : "") + "::" + Name;
}

UFunction* FRockDelegateInfo::GetSignatureFunction() const
{
	return SignatureFunction.Get();
}

UFunction* FRockFunctionInfo::GetSignatureFunction() const
{
	return Function.Get();
}
